#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include <iostream>
#include <string>

#include "tinyxml2.h"

using namespace std;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

using namespace tinyxml2;

XMLDocument* doc;
XMLElement* helpElement;

static cl::OptionCategory ClangExCategory("PFCtool's op");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp("\nMore help text...");

class ToXMLVisitor : public RecursiveASTVisitor<ToXMLVisitor> {
private:
    ASTContext *astContext; 

public:

    explicit ToXMLVisitor(CompilerInstance *CI) : astContext(&(CI->getASTContext())) {}

    virtual bool TraverseFunctionDecl(FunctionDecl *func) {
        helpElement = doc->NewElement( "function" );
        helpElement->SetAttribute( "name" , func->getNameInfo().getName().getAsString().c_str());
        helpElement->SetAttribute( "type" , func->getReturnType().getAsString().c_str());

        SourceManager& SM = astContext->getSourceManager();
        PresumedLoc PLoc = SM.getPresumedLoc(func->getSourceRange().getBegin());
        helpElement->SetAttribute( "line" , PLoc.getLine());

        XMLElement* auxElement;
        XMLElement* paramsElement;
        auxElement = helpElement;
        paramsElement = doc->NewElement( "params" );

        unsigned nParams = func->getNumParams();
        for(unsigned i = 0; i < nParams; i++){
            TraverseDecl(func->getParamDecl(i));
            paramsElement->InsertEndChild(helpElement);
        }
        
        auxElement->InsertEndChild(paramsElement);

        TraverseStmt(func->getBody());
        auxElement->InsertEndChild(helpElement);
        helpElement = auxElement;
        doc->InsertEndChild(helpElement);
        return true;
    }  

    virtual bool TraverseParmVarDecl(ParmVarDecl *parV) {
        helpElement =  doc->NewElement( "param" );
        helpElement->SetAttribute( "type", parV->getOriginalType().getAsString().c_str());
        helpElement->SetAttribute( "name" , parV->getIdentifier()->getNameStart());
        return true;
    }   

    virtual bool TraverseDeclStmt(DeclStmt *decSt){
        helpElement = doc->NewElement( "declarations" );
        XMLElement* auxElement;
        DeclStmt::decl_iterator init,end;

        for(init = decSt->decl_begin(), end = decSt->decl_end(); init!=end ; ++init){
            auxElement = helpElement;
            TraverseDecl(*init);
            auxElement->InsertEndChild(helpElement);
            helpElement = auxElement;
        }
        return true;
    }   

    virtual bool TraverseVarDecl(VarDecl *varD) {
        helpElement = doc->NewElement( "declaration" );
        helpElement->SetAttribute( "type" ,  varD->getType().getAsString().c_str());
        helpElement->SetAttribute( "name" ,  varD->getNameAsString().c_str());

        SourceManager& SM = astContext->getSourceManager();
        PresumedLoc PLoc = SM.getPresumedLoc(varD->getSourceRange().getBegin());
        helpElement->SetAttribute( "line" , PLoc.getLine());

        if(varD->hasInit()){
            XMLElement* auxElement = helpElement;
            TraverseStmt(varD->getInit());
            auxElement->InsertEndChild( helpElement );
            helpElement = auxElement;
        }
        return true;
    }   

    virtual bool TraverseCompoundStmt(CompoundStmt *cSt){
        helpElement =  doc->NewElement( "body" );
        XMLElement* auxElement;
        Stmt** body = cSt->body_begin();
        unsigned nStmts = cSt->size();
        for(unsigned i = 0; i< nStmts; i++){
            auxElement = helpElement;
            TraverseStmt(body[i]);
            auxElement->InsertEndChild(helpElement);
            helpElement = auxElement;
        }
        body = NULL;
        auxElement=NULL;
        return true;
    }  

    virtual bool TraverseIfStmt(IfStmt *ifst) {
        XMLElement* aux1Element = doc->NewElement( "if" );


        SourceManager& SM = astContext->getSourceManager();
        PresumedLoc PLoc = SM.getPresumedLoc(ifst->getIfLoc());
        aux1Element->SetAttribute( "line" , PLoc.getLine());

        XMLElement* aux2Element;
        XMLElement* auxBody;

        TraverseStmt(ifst->getCond());
        aux1Element->InsertEndChild( helpElement );
        
        aux2Element = doc->NewElement( "then" ); 
        TraverseStmt(ifst->getThen());
        
        if(Stmt::CompoundStmtClass!=ifst->getThen()->getStmtClass()){
            auxBody = doc->NewElement("body") ;
            auxBody->InsertEndChild( helpElement );
            aux2Element->InsertEndChild( auxBody );
        }else{
            aux2Element->InsertEndChild( helpElement );
        }
        
        aux1Element->InsertEndChild( aux2Element );
    
        if(ifst->getElse()!=NULL){

            aux2Element = doc->NewElement( "else" );
            TraverseStmt(ifst->getElse());

            if(Stmt::CompoundStmtClass!=ifst->getElse()->getStmtClass()){
                auxBody = doc->NewElement("body") ;
                auxBody->InsertEndChild( helpElement );
                aux2Element->InsertEndChild( auxBody );
            }else{
                aux2Element->InsertEndChild( helpElement );
            }

            aux1Element->InsertEndChild( aux2Element );
        }
        helpElement = aux1Element;
        return true;
    }  

    virtual bool TraverseWhileStmt(WhileStmt *whSt) {
        XMLElement* auxElement = doc->NewElement( "while" );

        SourceManager& SM = astContext->getSourceManager();
        PresumedLoc PLoc = SM.getPresumedLoc(whSt->getWhileLoc());
        auxElement->SetAttribute( "line" , PLoc.getLine());

        TraverseStmt(whSt->getCond());
        auxElement->InsertEndChild( helpElement );

        TraverseStmt(whSt->getBody());
        auxElement->InsertEndChild(helpElement);

        helpElement = auxElement;
        return true;
    }   

    virtual bool TraverseForStmt(ForStmt *forSt) {
        XMLElement* auxElement = doc->NewElement( "for" );

        SourceManager& SM = astContext->getSourceManager();
        PresumedLoc PLoc = SM.getPresumedLoc(forSt->getForLoc());
        auxElement->SetAttribute( "line" , PLoc.getLine());

        TraverseStmt(forSt->getInit());
        auxElement->InsertEndChild( helpElement );

        TraverseStmt(forSt->getCond());
        auxElement->InsertEndChild( helpElement );

        TraverseStmt(forSt->getInc());
        auxElement->InsertEndChild( helpElement );

        TraverseStmt(forSt->getBody());
        auxElement->InsertEndChild(helpElement);

        helpElement = auxElement;
        return true;
    }      

    virtual bool TraverseReturnStmt(ReturnStmt *ret) {
        XMLElement* auxElement = doc->NewElement( "return" );

        SourceManager& SM = astContext->getSourceManager();
        PresumedLoc PLoc = SM.getPresumedLoc(ret->getReturnLoc());
        auxElement->SetAttribute( "line" , PLoc.getLine());
        
        TraverseStmt(ret->getRetValue());
        auxElement->InsertEndChild(helpElement);
        helpElement =  auxElement;
        return true;
    }   

    virtual bool TraverseBinaryOperator(BinaryOperator* bOp){
        XMLElement* auxElement;
        cout << "hola" << endl;
        if(bOp->isAssignmentOp()){
            auxElement =  doc->NewElement( "assignment" );
            auxElement->SetAttribute( "name", cast<DeclRefExpr>(bOp->getLHS())->getDecl()->getDeclName().getAsString().c_str());
            
            SourceManager& SM = astContext->getSourceManager();
            PresumedLoc PLoc = SM.getPresumedLoc(bOp->getOperatorLoc());
            auxElement->SetAttribute( "line" , PLoc.getLine());

        }else{
            auxElement =  doc->NewElement( "binaryOperator" );
            auxElement->SetAttribute( "type", getOperatorType(bOp).c_str());
            auxElement->SetAttribute( "operator", bOp->getOpcodeStr().data() );
            TraverseStmt(bOp->getLHS());
            auxElement->InsertEndChild(helpElement);
        }
        TraverseStmt(bOp->getRHS());
        auxElement->InsertEndChild(helpElement);  
        helpElement =  auxElement;
        return true;
    }

    virtual bool TraverseCompoundAssignOperator(CompoundAssignOperator* caOp){
        XMLElement* auxElement;
        
        auxElement =  doc->NewElement( "assignmentOperator" );
        auxElement->SetAttribute( "name", cast<DeclRefExpr>(caOp->getLHS())->getDecl()->getDeclName().getAsString().c_str());
        auxElement->SetAttribute( "type", getOperatorType(caOp).c_str());
        string op (1,caOp->getOpcodeStr(caOp->getOpcode()).front());
        auxElement->SetAttribute( "operator", op.c_str());

        SourceManager& SM = astContext->getSourceManager();
        PresumedLoc PLoc = SM.getPresumedLoc(caOp->getOperatorLoc());
        auxElement->SetAttribute( "line" , PLoc.getLine());
        
        TraverseStmt(caOp->getRHS());
        auxElement->InsertEndChild(helpElement);  
        helpElement =  auxElement;
        return true;
    }

    virtual bool TraverseUnaryOperator(UnaryOperator* uOp){
        XMLElement* auxElement;
        if(uOp->isIncrementDecrementOp ()){
            auxElement =  doc->NewElement( "unaryOperator" );

            auxElement->SetAttribute( "name", cast<DeclRefExpr>(uOp->getSubExpr())->getDecl()->getDeclName().getAsString().c_str());
            
            if(uOp->isIncrementOp ()){auxElement->SetAttribute( "operator", "+" );}
            if(uOp->isDecrementOp ()){auxElement->SetAttribute( "operator", "-" );}
            if(!uOp->isIncrementDecrementOp ()){auxElement->SetAttribute( "operator", "other" );}

            SourceManager& SM = astContext->getSourceManager();
            PresumedLoc PLoc = SM.getPresumedLoc(uOp->getOperatorLoc());
            auxElement->SetAttribute( "line" , PLoc.getLine());
            helpElement =  auxElement;
        }
        if(uOp->isArithmeticOp()){ 
            if(uOp->getOpcode()==UO_Plus || uOp->getOpcode()==UO_Minus){
                auxElement = doc->NewElement( "signOperator" );
                auxElement->SetAttribute( "type", uOp->getOpcodeStr(uOp->getOpcode()).data());
                TraverseStmt(uOp->getSubExpr());
                auxElement->InsertEndChild(helpElement);
                helpElement =  auxElement;
            }else{
                auxElement = doc->NewElement( "notOperator" );
                TraverseStmt(uOp->getSubExpr());
                auxElement->InsertEndChild(helpElement);
                helpElement =  auxElement;
            }
        }
        return true;
    }

    string getOperatorType(BinaryOperator* bOp){
        if (bOp->isMultiplicativeOp()||bOp->isAdditiveOp()) 
            return "arithmetic" ;
        if (bOp->isComparisonOp()) 
            return "comparison" ;
        if (bOp->isLogicalOp()) 
            return "boolean" ;
        if (bOp->isCompoundAssignmentOp())
            if(bOp->getOpcode()>=BO_MulAssign  && bOp->getOpcode()<=BO_SubAssign) 
                return "arithmetic" ;
        return "default"; 
    }

    virtual bool TraverseImplicitCastExpr(ImplicitCastExpr *iCast) {
        cout << "caast" << endl;

        switch (iCast->getSubExpr()->getStmtClass()) {
        case Stmt::DeclRefExprClass:{
            helpElement =  doc->NewElement ( "variable" );
            helpElement->SetAttribute( "name" , cast<DeclRefExpr>(iCast->getSubExpr())->getDecl()->getDeclName().getAsString().c_str());
            return true;
        }
        default :{
                TraverseStmt(iCast->getSubExprAsWritten());
                return true;
            }
        }
    }

    virtual bool TraverseIntegerLiteral(IntegerLiteral *iLit) {
        helpElement =  doc->NewElement ( "const" );
        SmallString<8> aux;
        iLit->getValue().toStringSigned(aux);
        helpElement->SetAttribute( "value" , aux.c_str());
        return true;
    }

    virtual bool TraverseStringLiteral(StringLiteral *sLit) {
        cout << "striiiing" << endl;
        helpElement =  doc->NewElement ( "string" );
        sLit->getString();
        helpElement->SetAttribute( "value" , sLit->getString().str().c_str());
        return true;
    }

    virtual bool TraverseParenExpr(ParenExpr *parE) {
        TraverseStmt(parE->getSubExpr());
        return true;
    }

    virtual bool TraverseCallExpr(CallExpr *callE) {
        FunctionDecl* fun = callE->getDirectCallee();
        string name (fun->getNameInfo().getName().getAsString());

        if(string::npos!=name.find("Console"))
            return consoleCall(callE);

        helpElement =  doc->NewElement( "callFunction" );
        helpElement->SetAttribute("name" , name.c_str());
        helpElement->SetAttribute("type" , fun->getReturnType().getAsString().c_str());
        XMLElement* auxElement;
        unsigned numArgs = callE->getNumArgs();
        for(unsigned i=0; i<numArgs; i++){
            auxElement = helpElement;
            TraverseStmt(callE->getArg(i));
            XMLElement* argElement; 
            argElement = doc->NewElement( "arg" );
            argElement->InsertEndChild(helpElement);
            auxElement->InsertEndChild(argElement);
            helpElement = auxElement;
        }
        helpElement = auxElement;
        return true;
    }

    bool consoleCall(CallExpr *callE){
        FunctionDecl* fun = callE->getDirectCallee();
        string name (fun->getNameInfo().getName().getAsString());
        if(string::npos!=name.find("In_")) return cInCall(callE);
        if(string::npos!=name.find("Out_")) return cOutCall(callE);
        return true;
    }

    bool cInCall(CallExpr *callE){
        FunctionDecl* fun = callE->getDirectCallee();
        string name (fun->getNameInfo().getName().getAsString());
        helpElement = doc->NewElement("consoleIn");
        if(string::npos!=name.find("_Int")) helpElement->SetAttribute("type","int");
        if(string::npos!=name.find("_Bool")) helpElement->SetAttribute("type","bool");
        return true;
    }

    bool cOutCall(CallExpr *callE){
        FunctionDecl* fun = callE->getDirectCallee();
        string name (fun->getNameInfo().getName().getAsString());
        helpElement = doc->NewElement("consoleOut");

        SourceManager& SM = astContext->getSourceManager();
        PresumedLoc PLoc = SM.getPresumedLoc(callE->getLocStart());
        helpElement->SetAttribute( "line" , PLoc.getLine());

        XMLElement* auxElement = helpElement;
        cout << callE->getArg(0)->getStmtClassName() << endl;
        TraverseStmt(callE->getArg(0));
        auxElement->InsertEndChild(helpElement);
        helpElement = auxElement;
        return true;
    }

    virtual bool TraverseStmt(Stmt *S) {
        switch (S->getStmtClass()) {
        case Stmt::DeclStmtClass:
            return TraverseDeclStmt(cast<DeclStmt>(S));
        case Stmt::CompoundStmtClass:
            return TraverseCompoundStmt(cast<CompoundStmt>(S));
        case Stmt::BinaryOperatorClass:
            return TraverseBinaryOperator(cast<BinaryOperator>(S));
        case Stmt::CompoundAssignOperatorClass:
            return TraverseCompoundAssignOperator(cast<CompoundAssignOperator>(S));
        case Stmt::UnaryOperatorClass:
            return TraverseUnaryOperator(cast<UnaryOperator>(S));
        case Stmt::IfStmtClass:
            return TraverseIfStmt(cast<IfStmt>(S));
        case Stmt::WhileStmtClass:
            return TraverseWhileStmt(cast<WhileStmt>(S));
        case Stmt::ForStmtClass:
            return TraverseForStmt(cast<ForStmt>(S));
        case Stmt::ReturnStmtClass:
            return TraverseReturnStmt(cast<ReturnStmt>(S));
        case Stmt::ImplicitCastExprClass:
            return TraverseImplicitCastExpr(cast<ImplicitCastExpr>(S));  
        case Stmt::IntegerLiteralClass:
            return TraverseIntegerLiteral(cast<IntegerLiteral>(S));  
        case Stmt::StringLiteralClass:
            return TraverseStringLiteral(cast<StringLiteral>(S));
        case Stmt::ParenExprClass:
            return TraverseParenExpr(cast<ParenExpr>(S)); 
        case Stmt::CallExprClass:
            return TraverseCallExpr(cast<CallExpr>(S)); 
        default:
            return true;
        }
    }

    virtual bool TraverseTranslationUnitDecl(TranslationUnitDecl *tuD){
        DeclContext::decl_iterator init,end;
        init = cast<DeclContext>(tuD)->decls_begin();
        end = cast<DeclContext>(tuD)->decls_end();
        Decl *d ;
        while(init!=end){
            d=*init;    
            if(d->isFunctionOrFunctionTemplate()){
                string loc (d->getLocation().printToString(astContext->getSourceManager()));
                string builtins ("Builtins");
                if(string::npos==loc.find(builtins)){
                    TraverseDecl(d);
                }
            }
            init++;
        }
        return true;
    }    

};



class ExampleASTConsumer : public ASTConsumer {
private:
    ToXMLVisitor *visitor; 

public:
    explicit ExampleASTConsumer(CompilerInstance *CI) : visitor(new ToXMLVisitor(CI)) { }

    virtual void HandleTranslationUnit(ASTContext &Context) {
        visitor->TraverseDecl(Context.getTranslationUnitDecl());
    }
};



class ExampleFrontendAction : public ASTFrontendAction {
public:
    virtual ASTConsumer *CreateASTConsumer(CompilerInstance &CI, StringRef file) {
        return new ExampleASTConsumer(&CI); // pass CI pointer to ASTConsumer
    }
};



int main(int argc, const char **argv) {
    doc = new XMLDocument();
    const char *dirDoc=argv[argc-1];
    CommonOptionsParser op(argc, argv,ClangExCategory);    
    ClangTool Tool(op.getCompilations(), op.getSourcePathList());    

    Tool.run(newFrontendActionFactory<ExampleFrontendAction>());      

    //XMLElement* auxElement;
    //auxElement = doc->NewElement( "functions" );
    //auxElement->InsertEndChild( helpElement );
    //doc->InsertEndChild( auxElement );

    doc->InsertEndChild( helpElement );
    doc->SaveFile(dirDoc); 

    delete doc;

    return 0;
}

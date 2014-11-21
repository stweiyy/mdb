#include "exec.h"
#include "../util/trace.h"


int Executor::prepare(){
	Parse mdbParser;
	memset(&mdbParser,0,sizeof(mdbParser));

	runParser(&mdbParser,querystr.c_str());
	return 0;
}
int Executor::runParser(Parse *pParse,const char *sqlstr){
	assert(pParse!=NULL);
	pParse->zSql=sqlstr;

	extern void *mdbParserAlloc(void *(*mallocProc)(size_t));
	extern void mdbParserFree(void *p,void (*freeProc)(void*));
	extern int mdbParser(void *,int,Token,Parse *);
	extern void mdbParserTrace(FILE *TraceFILE,char *zTracePrompt);

	FILE *f=NULL;

	if(!parsertf.empty()){
		f=fopen(parsertf.c_str(),"w");
		printf("parse trace file: %s\n",parsertf.c_str());
		if(f==NULL){
			Tracer::tracePrint(ERROR,"open %s failed, %s",parsertf.c_str(),strerror(errno));
			exit(-1);
		}
		mdbParserTrace(f,(char *)"TRACE-");
	}
	
	void *pEngine=mdbParserAlloc(malloc);

	Lexer lexer(sqlstr);

	while(!lexer.ifReachEnd()){
		int flag=lexer.getNextToken();
		if(flag==1){
			printf("Reach query end!\n");
		}
		else{
			if(flag==0){
				printf("get Token:");
			}
			else{
				printf("Illegal char:");
			}
			char *tmp=(char *)malloc(lexer.getTkLen()+1);
			memset(tmp,0,lexer.getTkLen()+1);
			memcpy(tmp,lexer.getSqlStr()+lexer.getOffset(),lexer.getTkLen());

			printf("%s\n",tmp);

			Token t;
			t.z=reinterpret_cast<const unsigned char *>(tmp);
			t.dyn=0;
			t.n=lexer.getTkLen();
			int type=lexer.getTkType();
			free(tmp);
			switch(type){
				case TK_SPACE:
				case TK_COMMENT:
		//			printf("meet space or comment:\n");
					break;
				default:
					mdbParser(pEngine,type,t,pParse);

			}
		}
	}
	/*add the finish ; if not contained one, then token 0 */
	Token dummy;
	if(lexer.getTkType()!=TK_SEMI){
		mdbParser(pEngine,TK_SEMI,dummy,pParse);
	}
	mdbParser(pEngine,0,dummy,pParse);

	mdbParserFree(pEngine,free);

	mdbParserTrace(NULL,(char *)"");

	if(f!=NULL){
		fclose(f);
	}

	return 0;
}
void Executor::execute(){
	prepare();
	return;

}


/**
 * g++   exec.cpp  ../sqlparser/parser.cpp ../sqlparser/lexer.cpp  ../sqlparser/parsefunc.cpp  ../util/trace.cpp
 *
int main(int argc,char **argv){
	Executor ex("create  table t1(id int)");
	ex.setParsertf("parser.txt");
	ex.execute();
	return 0;
}
*/
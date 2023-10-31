
#include "./rtklib.h"

#define PROGNAME    "rnx2rtkp"          /* program name */
#define MAXFILE     8                   /* max number of input files */



int main()
{
	/*��ʼ��*/
	filopt_t fopt = { "" };
	FILE *fp;

	/*��������*/
	prcopt_t prcopt = prcopt_default;
	solopt_t solopt = solopt_default;
	prcopt.mode = PMODE_PPP_STATIC;

	prcopt.navsys = SYS_ALL;
	prcopt.sateph = EPHOPT_PREC;
	prcopt.refpos = 1;
	prcopt.glomodear = 0;
	solopt.timef = 1;
	solopt.posf = SOLF_LLH;
	
	

	/*��������ļ�*/
	strcpy(fopt.satantp, "..\\Data\\igs14.atx");
	strcpy(fopt.rcvantp, "..\\Data\\igs14.atx");
	strcpy(fopt.obs, "..\\Data\\algo0670.22o");
	strcpy(fopt.nav, "..\\Data\\brdc0670.22n");
	strcpy(fopt.sp3, "..\\Data\\gfz22002.sp3");
	strcpy(fopt.clk, "..\\Data\\gfz22002.clk");

	char* outfile = "..\\result\\ppp.pos";
	
	traceopen("..\\result\\ppp.trace");
	tracelevel(3);

	/*ppp����*/
	ppp_process(&prcopt, &solopt, &fopt,outfile);
	
	
}
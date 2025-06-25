#include "rtklib.h"

/* debug trace functions -----------------------------------------------------*/
#ifdef TRACE

static FILE *fp_trace = NULL;     /* file pointer of trace */
static int level_trace = 0;       /* level of trace */
static unsigned int tick_trace = 0; /* tick time at traceopen (ms) */

extern void traceopen(const char *file)
{
	if (!*file || !(fp_trace = fopen(file, "w"))) fp_trace = stderr;
	tick_trace = tickget();
}
extern void traceclose(void)
{
	if (fp_trace&&fp_trace != stderr) fclose(fp_trace);
	fp_trace = NULL;
}
extern void tracelevel(int level)
{
	level_trace = level;
}
extern void trace(int level, const char *format, ...)
{
	va_list ap;

	/* print error message to stderr */
	if (level <= 1) {
		va_start(ap, format); vfprintf(stderr, format, ap); va_end(ap);
	}
	if (!fp_trace || level>level_trace) return;
	fprintf(fp_trace, "%d ", level);
	va_start(ap, format); vfprintf(fp_trace, format, ap); va_end(ap);
	fflush(fp_trace);
}
extern void tracet(int level, const char *format, ...)
{
	va_list ap;

	if (!fp_trace || level>level_trace) return;
	fprintf(fp_trace, "%d %9.3f: ", level, (tickget() - tick_trace) / 1000.0);
	va_start(ap, format); vfprintf(fp_trace, format, ap); va_end(ap);
	fflush(fp_trace);
}
extern void tracemat(int level, const double *A, int n, int m, int p, int q)
{
	if (!fp_trace || level>level_trace) return;
	matfprint(A, n, m, p, q, fp_trace); fflush(fp_trace);
}
extern void traceobs(int level, const obsd_t *obs, int n)
{
	char str[64], id[16];
	int i;

	if (!fp_trace || level>level_trace) return;
	for (i = 0; i<n; i++) {
		time2str(obs[i].time, str, 3);
		satno2id(obs[i].sat, id);
		fprintf(fp_trace, " (%2d) %s %-3s rcv%d %13.3f %13.3f %13.3f %13.3f %d %d %d %d %3.1f %3.1f\n",
			i + 1, str, id, obs[i].rcv, obs[i].L[0], obs[i].L[1], obs[i].P[0],
			obs[i].P[1], obs[i].LLI[0], obs[i].LLI[1], obs[i].code[0],
			obs[i].code[1], obs[i].SNR[0] * 0.25, obs[i].SNR[1] * 0.25);
	}
	fflush(fp_trace);
}
extern void tracenav(int level, const nav_t *nav)
{
	char s1[64], s2[64], id[16];
	int i;

	if (!fp_trace || level>level_trace) return;
	for (i = 0; i<nav->n; i++) {
		time2str(nav->eph[i].toe, s1, 0);
		time2str(nav->eph[i].ttr, s2, 0);
		satno2id(nav->eph[i].sat, id);
		fprintf(fp_trace, "(%3d) %-3s : %s %s %3d %3d %02x\n", i + 1,
			id, s1, s2, nav->eph[i].iode, nav->eph[i].iodc, nav->eph[i].svh);
	}
	fprintf(fp_trace, "(ion) %9.4e %9.4e %9.4e %9.4e\n", nav->ion_gps[0],
		nav->ion_gps[1], nav->ion_gps[2], nav->ion_gps[3]);
	fprintf(fp_trace, "(ion) %9.4e %9.4e %9.4e %9.4e\n", nav->ion_gps[4],
		nav->ion_gps[5], nav->ion_gps[6], nav->ion_gps[7]);
	fprintf(fp_trace, "(ion) %9.4e %9.4e %9.4e %9.4e\n", nav->ion_gal[0],
		nav->ion_gal[1], nav->ion_gal[2], nav->ion_gal[3]);
}
extern void tracegnav(int level, const nav_t *nav)
{
	char s1[64], s2[64], id[16];
	int i;

	if (!fp_trace || level>level_trace) return;
	for (i = 0; i<nav->ng; i++) {
		time2str(nav->geph[i].toe, s1, 0);
		time2str(nav->geph[i].tof, s2, 0);
		satno2id(nav->geph[i].sat, id);
		fprintf(fp_trace, "(%3d) %-3s : %s %s %2d %2d %8.3f\n", i + 1,
			id, s1, s2, nav->geph[i].frq, nav->geph[i].svh, nav->geph[i].taun*1E6);
	}
}
extern void tracehnav(int level, const nav_t *nav)
{
	char s1[64], s2[64], id[16];
	int i;

	if (!fp_trace || level>level_trace) return;
	for (i = 0; i<nav->ns; i++) {
		time2str(nav->seph[i].t0, s1, 0);
		time2str(nav->seph[i].tof, s2, 0);
		satno2id(nav->seph[i].sat, id);
		fprintf(fp_trace, "(%3d) %-3s : %s %s %2d %2d\n", i + 1,
			id, s1, s2, nav->seph[i].svh, nav->seph[i].sva);
	}
}
extern void tracepeph(int level, const nav_t *nav)
{
	char s[64], id[16];
	int i, j;

	if (!fp_trace || level>level_trace) return;

	for (i = 0; i<nav->ne; i++) {
		time2str(nav->peph[i].time, s, 0);
		for (j = 0; j<MAXSAT; j++) {
			satno2id(j + 1, id);
			fprintf(fp_trace, "%-3s %d %-3s %13.3f %13.3f %13.3f %13.3f %6.3f %6.3f %6.3f %6.3f\n",
				s, nav->peph[i].index, id,
				nav->peph[i].pos[j][0], nav->peph[i].pos[j][1],
				nav->peph[i].pos[j][2], nav->peph[i].pos[j][3] * 1E9,
				nav->peph[i].std[j][0], nav->peph[i].std[j][1],
				nav->peph[i].std[j][2], nav->peph[i].std[j][3] * 1E9);
		}
	}
}
extern void tracepclk(int level, const nav_t *nav)
{
	char s[64], id[16];
	int i, j;

	if (!fp_trace || level>level_trace) return;

	for (i = 0; i<nav->nc; i++) {
		time2str(nav->pclk[i].time, s, 0);
		for (j = 0; j<MAXSAT; j++) {
			satno2id(j + 1, id);
			fprintf(fp_trace, "%-3s %d %-3s %13.3f %6.3f\n",
				s, nav->pclk[i].index, id,
				nav->pclk[i].clk[j][0] * 1E9, nav->pclk[i].std[j][0] * 1E9);
		}
	}
}
extern void traceb(int level, const unsigned char *p, int n)
{
	int i;
	if (!fp_trace || level>level_trace) return;
	for (i = 0; i<n; i++) fprintf(fp_trace, "%02X%s", *p++, i % 8 == 7 ? " " : "");
	fprintf(fp_trace, "\n");
}
#else
extern void traceopen(const char *file) {}
extern void traceclose(void) {}
extern void tracelevel(int level) {}
extern void trace(int level, const char *format, ...) {}
extern void tracet(int level, const char *format, ...) {}
extern void tracemat(int level, const double *A, int n, int m, int p, int q) {}
extern void traceobs(int level, const obsd_t *obs, int n) {}
extern void tracenav(int level, const nav_t *nav) {}
extern void tracegnav(int level, const nav_t *nav) {}
extern void tracehnav(int level, const nav_t *nav) {}
extern void tracepeph(int level, const nav_t *nav) {}
extern void tracepclk(int level, const nav_t *nav) {}
extern void traceb(int level, const unsigned char *p, int n) {}

#endif /* TRACE */
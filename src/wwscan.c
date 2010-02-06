/* World Wide Security - 2000
 * Trojan Scanner
 * Cassiano Aquino - (cassiano@wwsecurity.net)
 * CVS Id  : $Id: ebscan.c,v 1.2 2001/03/28 03:26:00 cassiano Exp $
 * CVS Log :
 *           $Log: ebscan.c,v $
 *           Revision 1.2  2001/03/28 03:26:00  cassiano
 *           *** empty log message ***
 *
 *           Revision 1.1.1.1  2001/03/26 05:44:25  cassiano
 *           Imported into the CVS repository
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <mysql/mysql.h>
#include <netinet/in.h>
#include <linux/ip.h>              
#include "ping.h"
#include <linux/icmp.h>
#include <netdb.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <setjmp.h>
#include <signal.h>


#define MAX_BUFF 500
#define MAX_TRY 4
#define UDP 1
#define TCP 0
#define RCFILE "scan.conf" 
#define NEW(x) (x *)malloc(sizeof(x))

typedef struct identos{
   char *nome,*valor;
   struct identos *prox;
}FILHO;

typedef struct{
   unsigned int tot;
   FILHO *inicio;
   FILHO *final;
}PAI;
PAI *root;

int errno;
char *TmpCGI;
int timeout;

sigjmp_buf p_environ ; /* process environment */


struct icmppacket 
{
        struct iphdr ip;
        struct icmphdr icmp;
};

MYSQL db;

//---


char *findcor(char *kisso)
{
   
 FILHO *node;
   unsigned int a;
   
   node=root->inicio;
   for(a=0;a<=root->tot-1;a++){
      if(!strncmp(kisso,node->nome,strlen(node->nome))){
              return strdup(node->valor);
              break;
      }
      node=node->prox;
   }
return(0);  
}
 
char *safe_getenv(char *var)
{
       return (getenv(var) == NULL) ? "": getenv(var);
}



int *faz_filho(char *nome,char *valor){
   FILHO *new;
   //aki q eu me ferro... o primeiro tem q ser mais chique que os outros..
   //pq ainda nao tem pointer pra lugar nenhum... entao ele eh o first e o last
   new=NEW(FILHO);
   new->nome=nome;
   new->valor=valor;
   new->prox=NULL; //eu sei sei sei ..........
   if(root->tot==0){
      root->inicio=root->final=new;
   }else{
      new->prox=root->inicio;
      root->inicio=new;
   }
   root->tot++;
   return(0);
}
int getrc (void) {
   FILE *fp;
   unsigned int a,i,b;
   char bufao[200],socorro[2][256];
   char *p1,*p2;
   //-------------------------------
   a=i=b=0;
   root=NEW(PAI);
   if(!root){
      perror("malloc");
      exit(0);
   }
   root->inicio=root->final=NULL;
   root->tot=0;
   
   fp=fopen(RCFILE,"r");
   if(!fp){
      if(!(fp=fopen(RCFILE,"w"))){
            fprintf(stderr,"Could not open %s\nExiting...\n",RCFILE);
           return(0);
      }
      fprintf(fp,"host:localhost\n");
      fprintf(fp,"database:searchtrojan\n");
      fclose(fp);
      fp=fopen(RCFILE,"r");
   }
   while(fgets(bufao,1024,fp)){
      //comentarios
      if(!strncmp(bufao,"#",1))
        continue;
      //nodes
      i=b=a=0;
      for(a=0;a<=strlen(bufao);a++){
         if(bufao[a]==':'){
            socorro[0][i]='\0';
            b=1;
            i=0;
         }else{
            if(b==1)
              socorro[1][i++]=bufao[a];
            else
              socorro[0][i++]=bufao[a];
         }
      }
      socorro[1][i-2]='\0'; //tira o \n junto...hehe
      p1=(char *) strdup(socorro[0]);
      p2=(char *) strdup(socorro[1]);
      faz_filho(p1,p2);
      bzero(socorro[0],256);
      bzero(socorro[1],256);
   }
   fclose(fp);
   return(1);
}

void restore_p_environ() {
     siglongjmp(p_environ, 1); /* restore the saved process environment */
}




int _err(int n,char *s) {
        fprintf (stderr, "Error : %s\n",s);
        if ( n == 1 )  exit (1);
        return(1);
}

int log(const char *host,char *what,int count)
{

        MYSQL_RES *logresult;
        MYSQL_ROW logcampo;
        char logquery[700];
        getrc();
        bzero(logquery,700);
        snprintf(logquery,700,"INSERT INTO %s (ip,what) values('%s','%s')",findcor("logtable"),host,what);
        if (mysql_query(&db,logquery))
        {       
                printf("ERRO: %s\n",mysql_error(&db));
        }
        if (count) {
                bzero(logquery,700);
                snprintf(logquery ,200,"UPDATE %s SET counter =  (counter + 1 )",findcor("counter"));
                if (mysql_query(&db,logquery))
                {       
                        printf("ERRO: %s\n",mysql_error(&db));
                }
        }
        strcpy(logquery,"SELECT counter FROM counter");
        if (mysql_query(&db,logquery))
        {
                fprintf(stdout,"Error: %s\n",mysql_error(&db));
                exit(1);
        } else {
                logresult = mysql_store_result(&db);
        }
        logcampo = mysql_fetch_row(logresult);
        return (int) logcampo[0];
}

long host_lockup(char * host)
{
        struct hostent * h;
        char s[16];
        h = gethostbyname(host);
        if (h == NULL) _err(1,strerror(errno));
        sprintf(s, "%d.%d.%d.%d", 
                 (unsigned char)h->h_addr_list[0][0],
                 (unsigned char)h->h_addr_list[0][1],
                 (unsigned char)h->h_addr_list[0][2],
                 (unsigned char)h->h_addr_list[0][3]
               );
        return inet_addr(s);
}

char *strstart(char *sstr, char *tstr)
{
        char *ret_str;
        ret_str = sstr;
        if ( sstr == NULL || tstr == NULL ) {
                return(NULL);
        }
        while ( *sstr != 0 && *tstr != 0 ) {
                if ( *sstr != *tstr ) {
                        return(NULL);
                }
                ++sstr;
                ++tstr;
        }
        if ( *tstr == 0 ) {
                return(ret_str);
        }
        return(NULL);
}


void get_cgi()
{
        int count;
        int i,j;

        count = atoi(safe_getenv("CONTENT_LENGTH"));

        TmpCGI = malloc(count+1);
        memset(TmpCGI,0,count+1);

        i = 0;
        do {
                j = fread( &TmpCGI[i], 1, count - i, stdin);
                if ( j >= 0 ) {
                        i += j;
                } else {
                        break;
                }
        } while (j > 0 && i < count );
}

int GetValue(char *source,char *dest,char *name,int dest_max)
{
        int i,j,k;
        memset(dest,0,dest_max);
        for(i=0; strstart(&source[i],name)!=&source[i] && source[i]!=0; ++i);

        if( source[i] != 0 ) {
                i+=strlen(name);
        } else {
                return( -1 );
        }

        for(k=0,j=i; source[j]!='&' && k<dest_max-1 && source[j]!=0;++k,++j) {
                if ( source[j] == '%' ) {
                        if ( source[j+1] == '0' && source[j+2]=='D'){
                                --k;
                        } else if (source[j+1]=='0'&&source[j+2]=='A'){
                                dest[k] = '\n';
                        }
                        j += 2;
                } else  if ( source[j] == '+' ) {
                        dest[k] = ' ';
                } else {
                        dest[k] = source[j];
                }
        }
        dest[k] = 0; 

        return(0);
}

int checkport(char * toprobe,int port,int proto) {
        struct sockaddr_in s;
        struct sockaddr_in from;
        struct icmppacket icmp;
        char * DATA = "eBrain TrojanScanner Working!";
        short int state = 0;
        long host = host_lockup(toprobe);
        short int i = 0;
        int sock_udp, sock_icmp, sock_tcp, len, size;

        s.sin_family = AF_INET;
        s.sin_port = htons (port);
        s.sin_addr.s_addr = host;


        if ( proto == TCP ) {
                if ( (sock_tcp = socket (AF_INET,SOCK_STREAM,0)) == -1 ) _err (1,strerror (errno));
				signal(SIGALRM, restore_p_environ); 
				if (sigsetjmp(p_environ, 1) == 0) { 
				    alarm(5);
                	if ( (connect (sock_tcp, (struct sockaddr *)&s, sizeof (s)) ) == -1) state = 1;
					alarm(0);
				}
        } 
        if ( proto == UDP ) {
                if ( ( sock_udp = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) < 0 ) _err (1,strerror (errno));
                if ( ( sock_icmp = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP) )  < 0 ) _err (1,strerror (errno));
                if ( fcntl ( sock_icmp, F_SETFL, O_NONBLOCK) == -1 ) _err (1,strerror (errno));
                if ( sendto (sock_udp, DATA, strlen (DATA) + 1, 0, (struct sockaddr *)&s,sizeof(s)) < 0) _err (1,strerror(errno));

                while(!state && i < MAX_TRY) {    
                        i++;
				        timeout = tpinghost(toprobe);
                		len = sizeof(from);
						//  icmp.icmp.type == ICMP_DEST_UNREACH && 
                        if ((size = recvfrom(sock_icmp,&icmp,sizeof(icmp),0,(struct sockaddr *)&from, &len)) >= 0) {
							if(host == icmp.ip.saddr && icmp.icmp.code == ICMP_PORT_UNREACH && icmp.icmp.type == ICMP_DEST_UNREACH) state = 1;
						} else sleep(5); 
                } 
        }
        return state;

}

char * language (char * lang, int  id) {
        MYSQL_RES *resultlang;
        MYSQL_ROW langrow;
        char query[200];
        bzero(query,200);
        snprintf(query,200,"SELECT content FROM %s WHERE language = '%s' AND id = %d",findcor("langtable"),lang,id);
        if (mysql_query(&db,query))
        {
                fprintf(stdout,"Error: %s\n",mysql_error(&db));
                exit(1);
        } else {
                resultlang = mysql_store_result(&db);
        }
        if ((langrow = mysql_fetch_row(resultlang))) { 
                return langrow[0];
        } else {
                return NULL;
        }
}

int main (int argc, char **argv) {
        MYSQL_RES *result;
        MYSQL_ROW campo;
        int encountered = 0, counter;
        char query[200];
        char *lang;
        char *hostname = safe_getenv("REMOTE_ADDR");
		char *pi = safe_getenv("PATH_INFO");
		if (hostname) hostname = strdup(hostname); 
		else exit(1);
	    if ( pi )  pi = strdup(pi);
		else exit(1);
	    if (pi && strstr(pi, "/pt/") != NULL) lang = "pt";
		else if (pi && strstr(pi, "/en/") != NULL) lang = "en";
		else { 
		   fprintf(stdout,"Location: /ebscan/index.cgi/pt/\n\n");
		   exit(0);
		}
        getrc();
        mysql_init(&db);
        mysql_options(&db,MYSQL_OPT_COMPRESS,0);
        mysql_options(&db,MYSQL_READ_DEFAULT_GROUP,"odbc");
        if (!mysql_real_connect (&db, findcor("dbhost"), findcor("username"),findcor("password"),findcor("database"),0,NULL,0)) 
        {
                fprintf(stdout,"Error: %s\n",mysql_error(&db));
                exit(1);
        }
        

        bzero(query,200);
        snprintf(query,200,"SELECT count(*) FROM %s ORDER BY protocol",findcor("listtable"));
        if (mysql_query(&db,query))
        {
                fprintf(stdout,"Error: %s\n",mysql_error(&db));
                exit(1);
        } else {
                result = mysql_store_result(&db);
        }
        campo = mysql_fetch_row(result);
		timeout = tpinghost(hostname);
		if (timeout <= 0)
		   timeout = 350;
        if (strstr(argv[0],"nph") == NULL) fprintf(stdout,"Content-type: text/html\n\n");
        snprintf(argv[0],100,"Trojan scanner running on target %s\n",hostname);
        fprintf(stdout,"<html>\n<head>");
        fprintf(stdout,"<title>%s</title>\n",language(lang,0));
        fprintf(stdout,"<link rel='stylesheet' type='text/css' href='/style/style.css'>");
        fprintf(stdout,"<script language='JavaScript'>\n");  
        fprintf(stdout,"checkbox0 = new Image();\n");
        fprintf(stdout,"checkbox0.src = '/gfx/checkbox0.gif';\n");
        fprintf(stdout,"checkbox1 = new Image();\n");
        fprintf(stdout,"checkbox1.src = '/gfx/checkbox1.gif';\n");
        fprintf(stdout,"radio0 = new Image();\n");
        fprintf(stdout,"radio0.src = '/gfx/radio0.gif';\n");
        fprintf(stdout,"radio1 = new Image();\n");
        fprintf(stdout,"radio1.src = '/gfx/radio1.gif';\n");
        fprintf(stdout,"</script>\n");
        fprintf(stdout,"</head>\n<body topmargin='0' leftmargin='0' rightmargin='0' oncontextmenu='return false' ondragstart='return false' onselectstart='return false'>");
        fprintf(stdout,"<table cellspacing='0' vspace='0' hspace='0' cellpading='0' width='100%%' border='0'>\n");
        fprintf(stdout,"<tr valign='middle' bgcolor='#c0c0c0'>\n<td>\n&nbsp;&nbsp;&nbsp;");
        fprintf(stdout,"<a class='osdn' href='http://www.wwsecurity.net'>wwsecurity</a>&nbsp;&middot;");
        fprintf(stdout,"<a class='osdn' href='http://www.screenshots.com.br'>screenshots</a>&nbsp;&middot;");
        fprintf(stdout,"</td>\n</tr>\n</table>");
        fprintf(stdout,"<table class='main' border='0' cellpadding='0' cellspacing='4' width='100%%'>\n");
        fprintf(stdout,"<tr><td class='header' align='center' width='100%%'>%s : %s</td></tr>\n",language(lang,1),hostname);
        fprintf(stdout,"<tr><td class='header' align='center' width='100%%'>%s : %d</td></tr>\n",language(lang,22),timeout);
        fprintf(stdout,"<tr><td class='header' align='center' width='100%%'>%s : %s</td></tr>\n",language(lang,2),(char *) campo[0]);
        bzero(query,200);
        snprintf(query,200,"SELECT protocol,count(*) FROM %s GROUP BY protocol",findcor("listtable"));
        if (mysql_query(&db,query))
        {
                fprintf(stdout,"Error: %s\n",mysql_error(&db));
                exit(1);
        } else {
                result = mysql_store_result(&db);
        }
        campo = mysql_fetch_row(result);
        fprintf(stdout,"<tr><td class='header' align='center' width='100%%'>%s : %s</td></tr>\n",language(lang,18),(char *) campo[1]);
        campo = mysql_fetch_row(result);
        fprintf(stdout,"<tr><td class='header' align='center' width='100%%'>%s : %s</td></tr>\n",language(lang,17),(char *) campo[1]);
        fprintf(stdout,"</table>\n");
        fprintf(stdout,"<table class='main' border='0' cellpadding='0' cellspacing='4' width='100%%'>\n");
        fprintf(stdout,"<tr><td class='protocol' align='center' width='100%%' colspan='3'>%s</td></tr>\n",language(lang,19));
        fprintf(stdout,"<tr><td class='header' align='center' width='20'>%s</td>\n",language(lang,16));
        fprintf(stdout,"<td class='header' align='center' width='100%%'>%s</td>\n",language(lang,3));
        fprintf(stdout,"<td class='header' align='center' width='20'>%s</td>\n",language(lang,8));
        bzero(query,200);
        fflush(stdout);
        snprintf(query,200,"SELECT name,port,protocol,id FROM %s WHERE protocol = 0 ORDER BY protocol AND port AND name",findcor("listtable"));
        if (mysql_query(&db,query))
        {
                fprintf(stdout,"Error: %s\n",mysql_error(&db));
                exit(1);
        } else {
                result = mysql_store_result(&db);
        }
        while((campo = mysql_fetch_row(result)))
        {
                fprintf(stdout,"</tr><td class='header' align='center' width='20%%'><img name='id_%s' src='/gfx/radio0.gif'></td>\n",(char *) campo[3]);
                fprintf(stdout,"<td class='scan' align='left' width='60%%'>%s</td>\n",(char *) campo[0]);
                fprintf(stdout,"<td class='header' align='center' width='20%%'><img name='chk_%s' src='/gfx/checkbox0.gif'></td></tr>",(char *) campo[3]);
                fflush(stdout);
        }
		if (timeout <= 300) {
	        fprintf(stdout,"<tr><td class='protocol' align='center' width='100%%' colspan='3'>%s</td></tr>\n",language(lang,20));
	        fprintf(stdout,"<tr><td class='header' align='center' width='20'>%s</td>\n",language(lang,16));
	        fprintf(stdout,"<td class='header' align='center' width='100%%'>%s</td>\n",language(lang,3));
	        fprintf(stdout,"<td class='header' align='center' width='20'>%s</td>\n",language(lang,8));
	        bzero(query,200);
	        fflush(stdout);
	        snprintf(query,200,"SELECT name,port,protocol,id FROM %s WHERE protocol = 1 ORDER BY protocol AND port AND name",findcor("listtable"));
	        if (mysql_query(&db,query))
	        {
	                fprintf(stdout,"Error: %s\n",mysql_error(&db));
	                exit(1);
	        } else {
	                result = mysql_store_result(&db);
	        }
	        while((campo = mysql_fetch_row(result)))
	        {
	                fprintf(stdout,"</tr><td class='header' align='center' width='20%%'><img name='id_%s' src='/gfx/radio0.gif'></td>\n",(char *) campo[3]);
	                fprintf(stdout,"<td class='scan' align='left' width='60%%'>%s</td>\n",(char *) campo[0]);
	                fprintf(stdout,"<td class='header' align='center' width='20%%'><img name='chk_%s' src='/gfx/checkbox0.gif'></td></tr>",(char *) campo[3]);
	                fflush(stdout);
	        }
		} else {
		   fprintf(stdout,"<tr><td class='protocol' align='center' width='100%%' colspan='3'>%s</td></tr>\n",language(lang,21));
		}
        fprintf(stdout,"</table>");
        fprintf(stdout,"<table class='main' border='0' cellpadding='0' cellspacing='4' width='100%%'>\n");
        fprintf(stdout,"<tr><td class='alert' align='center' width='100%%'>%s</td></tr>\n",language(lang,9));
        fprintf(stdout,"<tr><td class='header' align='center' width='100%%'><form name=form>%s<input readonly disabled type='text' size=3 name='encountered' value=0></form></td></tr>\n",language(lang,5));
        fprintf(stdout,"<tr><td class='header' align='center' width='100%%'>%s - %s</td></tr>\n",language(lang,14),(char *)log(hostname,"Scan",1));
        fprintf(stdout,"<tr><td class='header' align='center' width='100%%'>Cassiano Aquino (<a href='mailto:cassiano@wwsecurity.net'>cassiano@wwsecurity.net</a>) 2000</td></tr>\n");
        fprintf(stdout,"</table>\n");   
        bzero(query,200);
        fflush(stdout);
		if( timeout <= 300 ) {
	        snprintf(query,200,"SELECT name,port,protocol,id FROM %s ORDER BY protocol AND port AND name",findcor("listtable"));
		} else {
		    snprintf(query,200,"SELECT name,port,protocol,id FROM %s WHERE protocol = '0' ORDER BY protocol AND port AND name",findcor("listtable"));
		}
        if (mysql_query(&db,query))
        {
                fprintf(stdout,"Error: %s\n",mysql_error(&db));
                exit(1);
        } else {
                result = mysql_store_result(&db);
        }
        while((campo = mysql_fetch_row(result)))
        {
                fprintf(stdout,"<script>\n");
                fprintf(stdout,"parent.document.id_%s.src='/gfx/radio1.gif';\n",(char *) campo[3]); 
                if (!checkport(hostname,atoi((char *)campo[1]),atoi((char *) campo[2]))) {
                        encountered++;
                        counter = log(hostname,(char *) campo[0],0);
                        fprintf(stdout,"parent.document.chk_%s.src='/gfx/checkbox1.gif';\n",(char *) campo[3]);
                        fprintf(stdout,"parent.document.form.encountered.value='%d';\n",encountered);   
                        fprintf(stdout,"alert('%s %s %s %s %s %s');\n",(char *) campo[0],language(lang,8),language(lang,11),(char *) campo[1],language(lang,12),language(lang,15));
                }
                fprintf(stdout,"self.scrollBy(0,18);\n</script>");
                fflush(stdout);
        }
        fprintf(stdout,"\n</body>\n</html>");
        fflush(stdout);
        mysql_close (&db);
        exit(0);

}



#include "FUNCTIONS.h"

int child_number=-1; // αρίθμηση των διεργασιών-παιδιών 

void check_arg(int n,int argc)
{
	if ( (argc!=3) || n<=0 || n>11)
	{
		fprintf(stderr,"Απαιτούνται 2 θετικά ορίσματα.Ο αριθμός των παιδιών δεν πρέπει να ξεπερνάει το 10 \n");
		exit(EXIT_FAILURE);
	}	
}

void check_special(int k)
{
	if (k==0|| k==1){
		printf("Παραγοντικό του %d : 1\n",k);
	exit(0);
	}
}
// Ο πατέρας "κολλάει" με pause και "ξεκολλάει" οταν δεχτεί το sigint	
void handler(int sig)
{
	printf("Δέχτηκα το σήμα SIGINT,τερματίζω τα παιδιά\n");
}

// Είναι η fork() με έλεγχο όταν επιστρέφει -1
pid_t create_child(int i)
{
	pid_t c=fork();
	if (c<0)
	{
		perror("Error fork");
		exit(i);
	}
return c;
}

// creates n children
pid_t * create_children(int n)
{	
	pid_t *c;
	c=calloc(n,sizeof(int));
	for (int i=0;i<n;i++)
	{
		c[i]=create_child(i);
		if (c[i]==0)
		{
			child_number=i;
			break;
		}
	}	
return c;		
}


// Εδω πέρα δημιουργώ n+1 pipes 
int** create_pipes(int arithmos_paidion)
{
		int **p,i;
		
		p=malloc( (arithmos_paidion+1)*sizeof(int) );
			if (p==NULL)
			{
				perror("malloc error in p");
				exit(-1);
			}
		for(i=0;i<=arithmos_paidion+1;i++)
		{
			p[i]=malloc( 2*sizeof(int) );
				if(p[i]==NULL)
				{
					perror("Malloc error in p[i]");
					exit(i);
				}
			pipe(p[i]); //Thelei elegxo
			
		}
		
return p;
}


// Father writes count,paragontiko and then waits for signal SIGINT
void father_writes_first(int **pipe)
{
	signal(SIGINT,handler);
	
	close(pipe[0][0]);  	// Ο πατέρας κλείνει το read άκρο του pipe[0]
	int assos = 1;
	printf( "pipe[0][1]=%d\n",pipe[0][1]);
	write_to_pipe(pipe[0][1], assos); // Πατέρας γράφει το 1
		
	pause();
}

// write to pipe with error checking
void write_to_pipe(int f,int c)
{
	int a;
	printf("WRITE: Child=%d\n",child_number);
	a=write( f,&c,sizeof(c) );
	if ( a <0 )
			{
				fprintf(stderr, "Child %d failed to write :%s     fd=%d c=%d \n",child_number, strerror(errno),f,c);
				kill(getppid(),SIGINT);
			}
	else if (a==0)
		puts("Den Graftike tipota");
	printf("Graftikan %d bytes \n",a);
}

// read from pipe with error checking
int read_from_pipe(int fd)
{	
	int a,c;
		printf(" read child=%d      fd=%d \n",child_number,fd);
	if ( (a=read(fd,&c,sizeof(c) )<0) )
	{	
						fprintf(stderr, "Child %d failed to read :%s     fd=%d c=%d \n",child_number, strerror(errno),fd,c);
						kill(getppid(),SIGINT);
	}
	else if (a==0) // not an error but unexpected - end of file
	{	printf("Child =%d",child_number);
		perror(" Child read 0  (EOF) - Δε διάβασα τίποτα");
	}
	
return c;
}




	
// Only child gets here ->read from fifo and then writes
void Ypologismos_Paragontikou(pid_t *c,int n,int k,int **pipe)
{
		int paragontiko=-1,count=0;   // paragontiko->υπο κατασκευή παραγοντικό, count->πόσες φορές έχω κάνει πολ/σμο (αρα πόσες "πασες" μεταξυ των pipes)
		int i=1;       // μετράει πόσες φορές έχω μπει στην επανάληψη kai μετράει αν το c[0] θα διαβάσει από τον πατέρα ή απο c[n-1]
		
		
		while ( 1 ) //  TRUE 
		{  	
			// Το 1ο παιδί διαβάζει από πατέρα την 1η φορά
			if (child_number==0) 
			{
				if (i==0)
				{
					close(pipe[0][1]); //1ο Παιδί κλείνει writing end του πατέρα
					close(pipe[1][0]);//1ο Παιδί κλείνει reading end του επόμενου child
					paragontiko=read_from_pipe(pipe[0][0]);//Διαβάζω τον άσσο
					
				}
				else  // Τις άλλες φορές το 1ο παιδί διαβάζει από το n-1 παιδί
				{
					count=read_from_pipe(pipe[n][0]);
					paragontiko=read_from_pipe(pipe[n][0]);
				}
			}
			else // Όλα τα άλλα παιδιά διαβάζουν εδώ
			{
				close(pipe[child_number][1]);//Το Παιδί κλείνει writing end
				count=read_from_pipe(pipe[child_number][0]);//Διαβάζει απο το αντίστοιχο pipe το count
				paragontiko=read_from_pipe(pipe[child_number][0]);//Διαβάζει απο το αντίστοιχο pipe το paragontiko
			}

				
			
			count++;
			// πολλαπλασιάζω το "υπο κατασκευή παραγοντικό" με τον αντίστοιχο αριθμό  (1,2,3,4 ... k)
			paragontiko=paragontiko*count;	   
			
			
			// ελέγχω αν έφτασα το k! ,αυτό συμβαίνει όταν έχω μπει k φορες στην επαναληψη (άρα k πολλ/σμοι)
			if (count == k)				  
			{
				/* Βάζουμε εδώ περα την waitpid με WUNTRACED 
				προκειμένου να διασφαλίσουμε οτι ο πατέρας είναι σε παύση και περιμένει το SIGINT*/
				int wstatus;
				waitpid(getppid(),&wstatus,WUNTRACED); 
				kill(getppid(),SIGINT);
				printf(" To παραγοντικό του %d είναι %d \n",k,paragontiko);
				pause();
			}
			
			// Στη συνέχεια τα παιδάκια γράφουν
			write_to_pipe(pipe[child_number+1][1],count);
			write_to_pipe(pipe[child_number+1][1], paragontiko);
			i++;
		}
}

// Καταστρέφω το δέντρο διεργασιών-παιδιών και αποδεσμεύω τη δυναμικά δευσμευμένη μνήμη
void delete_all(pid_t *c,int n,int**pipes)
{
	int i;
// Κανένα από τα παιδιά δεν μπορεί να βγει απο τη συνάρτηση παραγοντικού,μονον ο πατέρας φτάνει εδώ όταν δεχτέι το σημα SIGINT
	for(i=0;i<n;i++)
	{	
		printf("skotono to c[%d]\n",i);
		kill(c[i],SIGTERM);
		free(pipes[i]);
	}
	free(pipes);
}


#include "FUNCTIONS.h"

int main ( int argc,char *argv[] )
{
	int n,k; // n->αριθμός παιδιών     fd->file descriptor που θα γίνουν με pipe
	pid_t *c;
// Έλεγχος για τα ορίσματα -> Τα θελω ίσα με 3
	n=atoi(argv[1]);
	check_arg(n,argc);

// k->θα βρουμε k!
	k=atoi(argv[2]);
	
// Ελεγχω ειδικες περιπτωσεις για τα παραγοντικά (0! και 1!)
	check_special(k);
	
//Φτιάχνω n+1 named pipes
	int **pipes;
	pipes=create_pipes(n);
	printf("Bgika apo create-pipes child=%d\n",child_number);
	
// Φτιάχνω το δέντρο,ορίζω αρίθμηση παιδιών σε global variable,paragontiko=1 και μετα μπαίνει σε pause(),περιμένει SIGINT 
	c=create_children(n);
	printf("Bgike apo fork() Child=%d\n",child_number);
// Father writes count=0,paragontiko=1 and then waits for signal SIGINT
	if(child_number==-1)
	{
	father_writes_first(pipes);
	printf("father egrapse \n");
	}


/*Ο πατέρας βρίσκεται σε pause(). Στη συνάρτηση αυτή μπαίνουν τα παιδια.Υπολογίζεται το 
παραγοντικο και εμφανίζεται στην οθόνη.Στη συνέχεια στέλνεται σημα στον πατέρα ο οποίος
* δεν μπαίνει στην συνάρτηση και προχωράει στον τερματισμό των παιδιών   */
Ypologismos_Paragontikou(c, n,k,pipes);

/*Καταστρέφω ότι έφτιαξα (n+1 named pipes,ένα φάκελο για τα pipes και "σκοτώνω" 
το δέντρο διεργασιών-παιδιών και αποδεσμεύω τη μνήμη που δέσμευσα με την calloc)  */
delete_all(c,n,pipes);


return 0;
}
	

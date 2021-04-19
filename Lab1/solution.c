#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(){ 
	int child[4],i,status; 
	child[0]=fork();          		   // Δημιουργώ το παιδί 0 (τοποθετείται κάτω αριστερά της αρχικής διεργασίας)
	if (child[0]<0){     	  	  	   // Έλεγχος για το αν δημιουργήθηκε σωστά το παιδί
		printf ( "error");    
	}
	if (child[0]==0){       	 	  // Μιλάω στο παιδί 0
		child[1]=fork();         	  // Δημιουργώ το παιδί 1 (τοποθετείται κάτω αριστερά του παιδιου 0)
			if (child[1]<0){ 	      // Έλεγχος για το αν δημιουργήθηκε σωστά
			printf ( "error");
			}
			else if (child[1]==0){    // Μιλάω στο παιδί 1
				for(i=0;i<10;i++){    // Το παιδί 1 εκτυπώνει 10 φορές το μήνυμα 
				printf("Process %d is executed,my father is %d\n",getpid(),getppid());
				sleep(1);      
				}
			exit(0);        		 // To παιδί 1 πεθαίνει,αφού εκπλήρωσε τη λειτουργία του
			}
		child[2]=fork();             // Δημιουργία του child[2] (τοποθετείται στα δεξιά του child[0])
			if (child[2]<0){     	 // Έλεγχος για το αν δημιουργήθηκε σωστά
				printf ( "error");   
			}
			if (child[2]==0){       // Το child[2] εμφανίζει 10 φορες ένα μηνυμα
				for(i=0;i<10;i++){
				printf("Process %d is executed,my father is %d\n",getpid(),getppid());
				sleep(1);
			}
			exit(0);                // Το child[2] "πεθαίνει"
			}
		for(i=0;i<10;i++){          // Το child[0] εμφανιζει το μήνυμα
			printf("Process %d is executed,my father is %d\n",getpid(),getppid());
			sleep(1);
			}
		wait(&status);              // Το child[0] περιμένει να πεθάνουν τα παιδιά του πριν συνεχίσει 
		exit(0);                    // Το child[0]     πεθαίνει
	}
	child[3]=fork();                // Το child[3] δημιουργείται (δεξία του child[0])
	if (child[3]<0) {               // Έλεγχος
		printf ( "error");
	}
	if (child[3]==0){              // Αν βρισκόμαστε στο child[3]...
		child[4]=fork();           // Δημιουργία του child[4]
		if (child[4]<0) {          // Έλεγχος
		printf ( "error");
		}
		if (child[4]==0){         // Αν βρισκόμαστε στο child[4]
			for(i=0;i<10;i++){    // Το child[4] εκτυπώνει το μήνυμα
			printf("Process %d is executed,my father is %d\n",getpid(),getppid());
			sleep(1);
			}
		exit(0);                  // To child[4] "πεθαίνει" 
		}
		for(i=0;i<10;i++){        // Το child[3] εκτυπώνει
			printf("Process %d is executed,my father is %d\n",getpid(),getppid());
			sleep(1);
			}
		wait(&status);            // Το child[3] περιμένει να πεθάνει το παιδί του ( child[4] )
		exit(0);                  // To child[3] "πεθαίνει"
	}
	wait(&status);                // Ο "πατέρας"(αρχική διεργασία) "περιμένει να πεθάνουν" τα παιδιά του
return 0;
}

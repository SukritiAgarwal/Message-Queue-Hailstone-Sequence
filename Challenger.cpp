#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;

int main(){
	int qid = msgget( ftok(".",'u'), IPC_EXCL | IPC_CREAT | 0600 );

    if (qid < 0){
        cout << "Challenger, Q create FAIL!\n";
	    qid = msgget(ftok(".",'u'), IPC_EXCL | 0600 );
    } 

    else
		cout << "Challenger, Q create SUCCESS\n";

	struct buf{
		long mtype;
		char g[5];
	};	

	buf msg;
	msg.mtype = 0;
	long location = 0;
	int size = sizeof(msg)-sizeof(long);

	do {
		if( msg.mtype % 2 == 1 )
			msg.mtype = (3 * msg.mtype) + 1;
		else 
			msg.mtype = msg.mtype / 2;

		cout << "Challenger, Checking queue...\n";
		msgrcv( qid, (struct msgbuf *)&msg, size, msg.mtype, msg.mtype );

	    cout << "Challenger,Received: " << msg.mtype << endl;

	    if( msg.mtype == 1 ) break;

	    if( msg.mtype % 2 == 1 )
			msg.mtype = (3 * msg.mtype) + 1;
		else 
			msg.mtype = msg.mtype / 2;

        if( msgsnd( qid, (struct msgbuf *)&msg, size, msg.mtype ) < 0 )
            cout << "Challenger, send FAIL!\n";
        else 
        	cout << "Challenger,Sending: " << msg.mtype << endl;

	}while (msg.mtype != 1);

	cout << "Challenger, Removing Q\n";

	if(msgctl (qid, IPC_RMID, NULL))
	    cout << "Challenger, Q remove FAIL!\n";
	else
	    cout << "Challenger, Q remove SUCCESS!\n";

	exit(0);
}

#include "hmm.h"


int main(int argc, char *argv[])
{
	char *modellist= argv[1]; 
	HMM MODELS[NUM_MODEL];
	load_models(modellist,MODELS,NUM_MODEL);

	char *filename=argv[2]; 
   	FILE *fp = open_or_die(filename, "r");
   	Observations O;
   	int S=0;
   	char token[OBSERV_IN_1SAMPLE] = "";
   	while( fscanf( fp, "%s", token ) > 0 ){
      if( token[0] == '\0' || token[0] == '\n' ) continue;
      char *tem = malloc( strlen(token) * sizeof(char) );
      strcpy(tem,token);
      O.sample[S]=tem;
      S=S+1;
   	};

   	Answers ANS;
   	for(int s=0;s<S;s++)
   	{
   		char *sample = O.sample[s];
   		int A = (int)'A';
	    int S0=(int)sample[0]-A;
   		double points[NUM_MODEL];
   		for(int m=0;m<NUM_MODEL;m++)
   		{	
	        double delta[NUM_STATE][OBSERV_IN_1SAMPLE];
	         for(int i=0;i<NUM_STATE;i++){
	            for(int j=0;j<OBSERV_IN_1SAMPLE;j++){
	               delta[i][j]=0;
	            }            
	         }

	        for(int i=0;i<NUM_STATE;i++)
	        {
	        	delta[i][0]=MODELS[m].initial[i]*MODELS[m].observation[S0][i];
	        }

	        for(int t=1;t<OBSERV_IN_1SAMPLE;t++)
	        {
	        	int o=(int)sample[t]-A;
	        	for(int n=0;n<NUM_STATE;n++)
	        	{
	        		double max,tem;
	        		max=0;
	        		tem=0;
	        		for(int n_1=0;n_1<NUM_STATE;n_1++)	
	        		{
	        			tem=delta[n_1][t-1]*MODELS[m].transition[n_1][n];
	            		if(tem>max)
	        			{
	        				max=tem;
	        			}
	        		}
	        		delta[n][t]=max*MODELS[m].observation[o][n];
	        	}
	        }

	        double P=0;
	        for(int i=0;i<NUM_STATE;i++)
	        {
	        	double tem=delta[i][(OBSERV_IN_1SAMPLE-1)];
	        	if(tem > P){
	        		P = tem;
	        	}
	        }
	        points[m]=P;
	        

    	}
    	
    	double MAX=0;int index=1;
    	for(int p=0;p<NUM_MODEL;p++)
    	{
    		if(MAX < points[p]){
    			MAX = points[p];
    			index=p+1;
    		}
    	}
	
    	ANS.name[s]=index;
    	ANS.points[s]=MAX;   	
    }
    FILE *f = fopen(argv[3], "w");
    for(int s=0;s<S;s++){
		fprintf(f, "model_0%d.txt %12.5e \n", ANS.name[s], ANS.points[s]);
    }
	fclose(f);


	
		char *testing_answer="testing_answer.txt"; 
	   	FILE *fp_testing_answer = open_or_die(testing_answer, "r");
	   	char answer[12] = "";
	   	int c=0;double acc=0;
	   	while( fscanf( fp_testing_answer, "%s", answer ) > 0 ){
		   char result[13] = "";
	   	  sprintf(result, "model_0%d.txt", ANS.name[c] );	
	       if(strcmp(result,answer)==0){
	       	acc=acc+1;
	       }
	       c=c+1;
	   	};
	   	acc=acc/c;
		fclose(fp_testing_answer);

	    FILE *f_accuracy = fopen("acc.txt", "w");
		fprintf(f_accuracy, "%f", acc);
		fclose(f_accuracy);
	
}




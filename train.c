#include "hmm.h"


int main(int argc, char *argv[])
{

   char *init= argv[2]; 
	HMM hmm_initial;
	loadHMM(&hmm_initial, init);

	char *filename=argv[3]; 
   FILE *fp = open_or_die(filename, "r");
   Observations O;
   int S=0;
   char token[OBSERV_IN_1SAMPLE] = "";
   while( fscanf( fp, "%s", token ) > 0 ){
      char *tem = malloc( strlen(token) * sizeof(char) );
      strcpy(tem,token);
      O.sample[S]=tem;
      S=S+1;
   };

   /* Load initial model & training data */
   int ITERATION;
   ITERATION=atoi(argv[1]); 
   double GAMMA[NUM_STATE][OBSERV_IN_1SAMPLE],EPSILON[(OBSERV_IN_1SAMPLE-1)][NUM_STATE][NUM_STATE],Bjk[NUM_OBSERV][NUM_STATE];
   for(int I=0;I<ITERATION;I++)
   {

         for(int i=0;i<NUM_STATE;i++){
            for(int j=0;j<OBSERV_IN_1SAMPLE;j++){
               GAMMA[i][j]=0;
            }            
         }
         for(int t=0;t<(OBSERV_IN_1SAMPLE-1);t++)
         {
            for(int i=0;i<NUM_STATE;i++){
               for(int j=0;j<NUM_STATE;j++){
                  EPSILON[t][i][j]=0;
               }            
            }
         }
         for(int i=0;i<NUM_OBSERV;i++){
            for(int j=0;j<NUM_STATE;j++){
               Bjk[i][j]=0;
            }            
         }
       
      
      for(int s=0;s<S;s++)
      {
         double alpha[NUM_STATE][OBSERV_IN_1SAMPLE],beta[NUM_STATE][OBSERV_IN_1SAMPLE],
         gamma[NUM_STATE][OBSERV_IN_1SAMPLE],epsilon[(OBSERV_IN_1SAMPLE-1)][NUM_STATE][NUM_STATE],bjk[NUM_OBSERV][NUM_STATE];
         for(int i=0;i<NUM_STATE;i++){
            for(int j=0;j<OBSERV_IN_1SAMPLE;j++){
               alpha[i][j]=0;
            }            
         }
         for(int i=0;i<NUM_STATE;i++){
            for(int j=0;j<OBSERV_IN_1SAMPLE;j++){
               beta[i][j]=0;
            }            
         }
         for(int i=0;i<NUM_STATE;i++){
            for(int j=0;j<OBSERV_IN_1SAMPLE;j++){
               gamma[i][j]=0;
            }            
         }
         for(int t=0;t<(OBSERV_IN_1SAMPLE-1);t++)
         {
            for(int i=0;i<NUM_STATE;i++){
               for(int j=0;j<NUM_STATE;j++){
                  epsilon[t][i][j]=0;
               }            
            }
         }
         for(int i=0;i<NUM_OBSERV;i++){
            for(int j=0;j<NUM_STATE;j++){
               bjk[i][j]=0;
            }            
         }

         int A = (int)'A';
         //alpha
         char *sample = O.sample[s];
         int S0=(int)sample[0];
         S0=S0-A;
         for(int i=0;i<NUM_STATE;i++)
         {
            alpha[i][0]=hmm_initial.initial[i]*hmm_initial.observation[S0][i];
         }
         for(int t=1;t<OBSERV_IN_1SAMPLE;t++)
         {
            int o=(int)sample[t]-A;
            for(int n=0;n<NUM_STATE;n++){
               double tem=0;
               for(int n_1=0;n_1<NUM_STATE;n_1++){
                  tem=tem+alpha[n_1][t-1]*hmm_initial.transition[n_1][n]*hmm_initial.observation[o][n];
               }
               alpha[n][t]=tem;
            }
         }
         //beta 
         for(int i=0;i<NUM_STATE;i++)
         {
            beta[i][(OBSERV_IN_1SAMPLE-1)]=1;
         }
         int t=(OBSERV_IN_1SAMPLE-1)-1;
         while(t>=0){
            for(int n=0;n<NUM_STATE;n++){
               double tem=0;
               for(int n_1=0;n_1<NUM_STATE;n_1++){
                   int o=(int)sample[t+1]-A;
                  tem=tem+beta[n_1][t+1]*hmm_initial.transition[n][n_1]*hmm_initial.observation[o][n_1];
               }
               beta[n][t]=tem;
            }
            t=t-1;
         }

         //gamma
         for(int t=0;t<OBSERV_IN_1SAMPLE;t++)
         {
            double tem=0;
            for(int n=0;n<NUM_STATE;n++)
            {
               tem=tem+alpha[n][t]*beta[n][t];
            }
            for(int n=0;n<NUM_STATE;n++)
            {
               gamma[n][t]=(alpha[n][t]*beta[n][t])/tem;
            }
         }
         // epsilon
         for(int t=0;t<OBSERV_IN_1SAMPLE-1;t++)
         {
            int o=(int)sample[t+1]-A;
            double tem=0;
            for(int i=0;i<NUM_STATE;i++)
            {
               for(int j=0;j<NUM_STATE;j++)
               {
                  tem=tem+alpha[i][t]*hmm_initial.transition[i][j]*hmm_initial.observation[o][j]*beta[j][t+1];
               }
            }
            for(int i=0;i<NUM_STATE;i++)
            {
               for(int j=0;j<NUM_STATE;j++)
               {
                 epsilon[t][i][j]=(alpha[i][t]*hmm_initial.transition[i][j]*hmm_initial.observation[o][j]*beta[j][t+1])/tem;
               }
            }
         }

         //bjk
         for(int t=0;t<OBSERV_IN_1SAMPLE;t++)
         {
            int o=(int)sample[t]-A;
            for(int n=0;n<NUM_STATE;n++)
            {
               bjk[o][n]=bjk[o][n]+gamma[n][t];
            }
         }

         
         for(int x1=0; x1 < NUM_STATE; x1++)
         {
            for(int x2=0; x2 < OBSERV_IN_1SAMPLE; x2++)
            {
               GAMMA[x1][x2]=GAMMA[x1][x2]+gamma[x1][x2];
            }
         }




         for(int t=0 ; t < (OBSERV_IN_1SAMPLE-1) ; t++)
         {
            for(int x1=0 ; x1 < NUM_STATE ; x1++)
            {
               for(int x2=0; x2 < NUM_STATE ; x2++ )
               {
                  EPSILON[t][x2][x1]=EPSILON[t][x2][x1]+epsilon[t][x2][x1];
               }
            }
         }



         for(int x1=0;x1<NUM_OBSERV;x1++)
         {
            for(int x2=0;x2<NUM_STATE;x2++)
            {
                  Bjk[x1][x2]=Bjk[x1][x2]+bjk[x1][x2];
            }
         }


      }

      for(int i=0;i<NUM_STATE;i++)
      {
         hmm_initial.initial[i]=GAMMA[i][0]/S;
      }
      for(int i=0;i<NUM_STATE;i++)
      {
         for(int j=0;j<NUM_STATE;j++)
         {
            double E=0;double G=0;
            for(int t=0;t<(OBSERV_IN_1SAMPLE-1);t++)
            {
               E=E+EPSILON[t][i][j];
               G=G+GAMMA[i][t];
            }
            hmm_initial.transition[i][j]=E/G;
         }  
      }
      for(int i=0;i<NUM_STATE;i++){
         double G=0;
         for(int t=0;t<OBSERV_IN_1SAMPLE;t++){
            G=G+GAMMA[i][t];
         }
         for(int j=0;j<NUM_OBSERV;j++){
            hmm_initial.observation[j][i]=Bjk[j][i]/G;
         }
      }




   }
      

   char *model= argv[4]; 
   FILE *export = open_or_die(model, "w");
   dumpHMM(export,&hmm_initial);

}




















#include <stdio.h>
#include <stdlib.h>							
#include <string.h>
#include <dirent.h>					/* YÝGÝT BURDURLU */	
#include <unistd.h>
#include <ctype.h>						/* 25.12.2016 */	
struct files{
	char *file_name;		/*This program takes 2 char *argv first is directory path second one command.txt path and its*/
	struct words *word;		/* returns and output.txt to current directory */
 	struct files *next;	       	/* !! PROGRAM WORKS PERFECTLY I HOPE THAT ÝTS NOT RETURNS AND ERROR ,IF IT WOULD GIVE AN ERROR */					
};						/* PLEASE TRY TO EXECUTE ,AGAIN AND AGAIN !!!*/
struct words{
	char *word;			
	int count;		/*Program keeps data in linked lists , there is 2 main linklist first binary tree linkedlist*/
	struct words *next;		/*second linkedlist in linkedlist type which means every link of files struct has its own */ 
};				/*linked list and that makes easier to save data */
struct tree{				
	char *word;		 
	int count;
	struct tree *left;
	struct tree *right;
};
int size_of(char* ary){
       char letters[] = " _./QWERTYUIOPASDFGHJKLZXCVBNMqwertyuýopasdfghjklizxcvbnm1234567890";
       int i=0,k=0,j=0,size =0;
       if(ary==NULL){
       	
       	return -10;
       }
       while(ary[j++]){}
       for(i=0;i<j;i++){
       	for(k=0;k<64;k++){
       		if(letters[k]==ary[i]){
       			size++;
       			continue;
		       }
	       }
       }
return size;     
}
char* make_Word_Lower(char *word){ /* its work is making Uppercase letters in a word Lowercase.*/
	int size = size_of(word),i;
	char letter,*newW=(char*) malloc(sizeof(char)*(size+1));
	strcpy(newW,word);
	for(i=0;i<size;i++){
		if(newW[i]>64&&newW[i]<91){	
			letter = word[i];
			letter += 32;
			newW[i]=letter;
			
		}
	}
	
	return newW;
}
char* min(struct tree *root){  /* RETURNS MÝN CHAR * OF BÝNARY TREE*/
	if(root->left==NULL){
		return root->word;
	}
	return min(root->left);
}
char* max(struct tree *root){	/* RETURNS MAX CHAR * OF BÝNARY TREE*/
	if(root->right==NULL){
		return root->word;
	}
	return max(root->right);
}
int search(struct tree *root,char *word){
	/* searching tree to return 0 (yes its in tree) or 1 (no its not in tree) */
	/* 2 means its return a error about NULL pointer */
	if(root==NULL){
		return 0;
	}
	else{
		if(strcmp(word,root->word)==0){
			return root->count;
		}
		if(strcmp(word,root->word)<0)/*word of node bigger than word*/{
  			return search(root->left,word);		      
		}
		return search(root->right,word);		
	}		
}
struct tree* remove_root(struct tree *root,char* word){ /* RETURNS A ROOT WHÝCH HAS BEEN LOST A ROOT ÝTS WORKS RECURSÝVELY HARD */
	   		       	         	      /* PART OF THE CODE ,ÝT REMOVE ROOT AND KEEP GOÝNG UNTÝL APPROACH TO */
	if(root==NULL){			      /* LEAVES AND RETURNS A NEW ROOT*/	
		return NULL;
	}
	if(strcmp(root->word,word)==0){
		
		if(root->left==NULL && root->right==NULL){
			return NULL;
		}
		if(root->right){
			root->word = min(root->right);
			root->count = search(root,min(root->right));
		 	root->right = remove_root(root->right,min(root->right));
		 	
			return root;
		}
		if(root->left){
			root->word = max(root->left);
			root->count = search(root,max(root->left));
			root->left = remove_root(root->left,max(root->left));
			return root;
		}		 	
	}
	if(strcmp(root->word,word)<0){ /*node word bigger than removable word*/
		root->right = remove_root(root->right,word);
		return root;
	}
	root->left = remove_root(root->left,word);
	return root;
	
	
}
void remove_func(struct tree *tr,char *word,struct files *file_head){ /* PROCESS REMOVE COMMAND AND SENDS WORD TO REMOVE FROM TREE*/
					       		/* AND REMOVE THE WORD FROM THE ALL OF FÝLE LÝNKS*/
	struct files *temp=file_head;
	struct words *temp_w;
	struct words *temp_w2;
	word = make_Word_Lower(word);
	if(search(tr,word)>0){ /* 0 means there no is a word like that in tree */
 	          tr = remove_root(tr,word);	
	}
	else{	
		return;
	}
	while(temp){
		temp_w = temp->word;
		temp_w2 = temp_w->next;
		if(strcmp(temp_w->word,word)==0){
			temp->word=temp_w2;
			free(temp_w);
		}
		while(temp_w2){
			if(strcmp(temp_w2->word,word)==0){
				temp_w->next=temp_w2->next;
				free(temp_w2);	        	
			}
			temp_w=temp_w->next;
			temp_w2=temp_w2->next;
		}
		temp=temp->next;
	}	
}
int find_Depth(struct tree *root,char *word,int depth){ /* DEPTH MEANS what number of tree braunch the root of word HAS BEEN ON */ 
	if(strcmp(root->word,word)==0){	
		return depth;
	}
	if(root->right){
	 	if(strcmp(root->word,word)<0){/*word bigger than word of root*/
				       depth++;
				       return find_Depth(root->right,word,depth);
                    }	
	}
	if(root->left){
		depth++;
		return find_Depth(root->left,word,depth);	
	}
	return 0;	
}
int textorFile(char *ary,char *path){
	 /*This func using ary for find text file or directory */
	 FILE *fin;
	 char *t;
	 int position,i;
	 if((fin = fopen(path,"r"))==NULL){
	 	printf("%s1\n",path);
 		return 0;
	 	
	 }	 
    	 fseek(fin,0L, SEEK_END);
    	 position = ftell(fin);
    	 t = (char*) malloc(position*sizeof(char));
    	 t[position] = '\0';
    	 fseek(fin,0L,SEEK_SET);
    	 fread(t,sizeof(char),position,fin);
	 
	if(position>100){
	position =100;
	}
	fclose(fin);
    	 for(i=0;i<position;i++){
    	 	if(isascii(t[i])==0){
   	 	
    	 		
    	 		return 0;
		     }
	     }
	  free(t);	  
	 return 1;
}
int linuxorwin(){ 
	 char t[2];
	 t[0]='a';
	 t[1]='\n';
	 
	 if(strlen(t)==2){
	 	return 1; /* it means compiler device Wiin*/
	 }
	 else{
	 	return 0; /* it means compiler device linux*/
	 }
}
void print_word(struct tree *root,struct files *file_head,char* word){ /* priting words information to file.txt and prompt*/
	struct words *temp;
	struct files *tempf=file_head;
	int in = search(root,word),total=0;
	FILE *fout = fopen("output.txt","a");
	if(in==0){

		return;
	}
	while(tempf){
		temp = tempf->word;
		while(temp){
			if(strcmp(word,temp->word)==0){
				total= total+ temp->count;
			}
			temp=temp->next;
		}
		tempf = tempf->next;
	}
	printf("%s\n",word);
	fprintf(fout,"%s\n",word);
	printf("\t%d\n\t%d\n",total,find_Depth(root,word,1));
	fprintf(fout,"\t%d\n\t%d\n",total,find_Depth(root,word,1));
	
	while(file_head){
		temp = file_head->word;
		while(temp){
			if(strcmp(word,temp->word)==0){
				printf("\t%s %d\n",file_head->file_name,temp->count);
				fprintf(fout,"\t%s %d\n",file_head->file_name,temp->count);
			}
			temp=temp->next;
		}
		file_head = file_head->next;
	}
	fclose(fout);
}
struct tree* add(struct tree *root,char *word){
	/* if word not NULL and not equal a word in tree it will add it to tree in a place*/
	/* if equal it will add  +1 to number of count*/
	struct tree *node;
	
	if(word==NULL){
		printf("null word /add");
		return NULL;
	}
	
	
	if(root == NULL){
		node = (struct tree*) malloc(sizeof(struct tree));
		node->count = 1;
		node->left = NULL;
		node->right = NULL;
		node->word = word;
		
		return node;
	}
	
	if(root->word==NULL){ /* first char* of tree*/
		root->word = word;
		root->count = 1;
		root->left = NULL;
		root->right = NULL;
		
		return root;
	}
	
	
	if(strcmp(root->word,word)<0){ /* word bigger than word of node then its going to place right of node*/
 	   
		root->right = add(root->right, word);
		return root;
	}
	else if(strcmp(root->word,word)>0){/* word less than word of node then its going to place left of node*/
	
		root->left = add(root->left, word);
		return root; 
	}
	else{
		++root->count;
		return root;
	}
}
void add_File_to_Link(struct files *curr,char *file_name){ /*its adding a new link to  files linklist*/
	struct files *file;
	while(curr->next){
		curr=curr->next;
	}
	file = (struct files*) malloc(sizeof(struct files));
	file->file_name = (char*) malloc(sizeof(char)*(size_of(file_name)+1));
	strcpy(file->file_name,file_name);
	file->next=NULL;
	file->word = (struct words*) malloc(sizeof(struct words));
	file->word->word = NULL;

	curr->next = file;
	
}
void add_Word_to_link(struct files *file,struct words *curr,char *word){
	/*its looking to file link if there is the word in link unless its allocating new link and adding to link of file*/
	struct words *new_W;
	struct words *temp = curr;
	while(temp){
		if(temp->word==NULL){
			return;
		}
		if(strcmp(word,temp->word)==0){
			temp->count++;
			return;
		}
		temp = temp->next;
	}
	temp = curr;
	while(temp->next){
		temp=temp->next;}
	new_W = (struct words*) malloc(sizeof(struct words));
	new_W->word = (char*) malloc(sizeof(char)*(size_of(word)+1));
	strcpy(new_W->word,word);
	new_W->count = 1;
	new_W->next = NULL;
	temp->next = new_W;
	
	
	

}
void send_Word_l_T(struct files *head,struct tree *tr,char *word){ /*its processing the word for many process those adding tree , adding*/
	struct files *file =head;			/* files linklist by adding word link  and sending them functions*/
	if(head){
		
		while(file->next){
			
   			file = file->next;
		}
	}	
	word = make_Word_Lower(word);     
	tr = add(tr,word);	
	if(file){
		if(file->word->word==NULL){ /*head of file list word*/
			file->word->word = (char*) malloc(sizeof(char)*(size_of(word)+1));
			strcpy(file->word->word,word);
			file->word->count = 1;
			file->word->next = NULL;
		}
		else{
			add_Word_to_link(file,file->word,word);
		}
	}
	
	
	
}
char** fileread(struct tree *tr,struct files *file_head ,char *filepath,char* filename){
	 int position,i,l=0,m=0,k=0;
    	 FILE *fin= fopen(filepath,"r");		/* its just reading file DYNAMÝCALY AND  sending words to related functions*/
    	 char *t,**he,*newt;	 
    	 fseek(fin,0L, SEEK_END);
    	 position = ftell(fin);
    	 t = (char*) malloc(position*sizeof(char));
    	 t[position] = '\0';
    	 fseek(fin,0L,SEEK_SET);
    	 fread(t,sizeof(char),position,fin);
    	 if(linuxorwin()==1){
    	  		 for(i=0;i<position;i++){		    
		        	     if(t[i]=='\n'){
    	 		     		position--;
                     	}
           	 }
 	 }
	 newt=(char*) malloc(sizeof(char)*(position+1));
	 strncpy(newt,t,position);
	 newt[position] ='\0'; 
	 he = (char**) malloc(sizeof(char*));
    	 he[0]= (char*) malloc(sizeof(char));
           for(i=0;i<position;i++){
    		     
	          	        /*not number*/ /*not upper case*/    /*not lower case*/ 
		     if((t[i]<48 || (t[i]>57&&t[i]<65) || (t[i]>90&&t[i]<97)||(t[i]>122))&& k ==1){/*not word*/
		    
    			     he[l][m]='\0';
    			     l++;
	     	     	     he = (char**) realloc(he,(l+1)*sizeof(char*));
    			     he[l]= (char*) malloc(sizeof(char));
    			     m=0;
			     k=0;    }
			     /*number*/               /*upper case*/       /*lower case*/         
	     	     else if((t[i]>47 && t[i]<58) || (t[i]>64 && t[i]<91)||(t[i]>96 && t[i]<123)){/*word or number*/
	     	    
    		               he[l]= (char*) realloc(he[l],(i+2)*sizeof(char));
		               he[l][m]=t[i];
		               m++;
		               k=1;  }       		       
	 }
  	 he[l][m] ='\0';
	 he = (char**) realloc(he,(l+2)*sizeof(char*));
  	 he[l+1] = (char*)malloc(sizeof(char));
  	 he[++l][0]= '\0';
  	   	    if(file_head->file_name==NULL){/* first add link */
			        file_head->next=NULL;
			        file_head->file_name = (char*) malloc(sizeof(char)*(size_of(filename)+1));
			        strcpy(file_head->file_name,filename);
			        file_head->word = (struct words*) malloc(sizeof(struct words));
			        file_head->word->word = NULL;
  		    }
           	    else{
           	         	        add_File_to_Link(file_head,filename);
                        }
  	 	    for(i=0;i<l;i++){
     	  	
 	 	    		 if(he[i][0]!='\0'){
  		      				send_Word_l_T(file_head,tr,he[i]); /* here is sending*/
       				}
			}
           fclose(fin);
  	 free(t);
  	 free(newt);
	 return he;
}
char** fileread2(char* filepath){   /* its reading command.txt dynamicaly */
	int position,i,l=0,m=0,k=0;
    	 FILE *fin= fopen(filepath,"r");
    	 char *t,**he,*newt;	 
    	 fseek(fin,0L, SEEK_END);
    	 position = ftell(fin);
    	 t = (char*) malloc(position*sizeof(char));
    	 t[position] = '\0';
    	 fseek(fin,0L,SEEK_SET);
    	 fread(t,sizeof(char),position,fin);
    	 if(linuxorwin()==1){
    	  		 for(i=0;i<position;i++){		    
		        	     if(t[i]=='\n'){
    	 		     		position--;
                     	}
           	 }
 	 }
	 newt=(char*) malloc(sizeof(char)*(position+1));
	 strncpy(newt,t,position);
	 newt[position] ='\0'; 
	 he = (char**) malloc(sizeof(char*));
    	 he[0]= (char*) malloc(sizeof(char));
           for(i=0;i<position;i++){
	          	        /*not number*/ /*not upper case*/    /*not lower case*/ 
		     if((t[i]=='\n'||t[i]==' '||t[i]=='\0')&& k ==1){/*not word*/
    			     he[l][m]='\0';
    			     l++;
	     	     	     he = (char**) realloc(he,(l+1)*sizeof(char*));
    			     he[l]= (char*) malloc(sizeof(char));
    			     m=0;
			     k=0;    }
			     /*number*/               /*upper case*/       /*lower case*/         
	     	     else{/*word or number*/
    		               he[l]= (char*) realloc(he[l],(i+2)*sizeof(char));
		               he[l][m]=t[i];
		               m++;
		               k=1;  }
	 }
  	 he[l][m] ='\0';
	 he = (char**) realloc(he,(l+2)*sizeof(char*));
  	 he[l+1] = (char*)malloc(sizeof(char));
  	 he[++l][0]= '\0';
  	 
  	 return he;
}
void lookforTexts(struct tree *tr,struct files *file_head,char *path){ /* the most important part of code : its reading directory recursively*/
     		     		       		   /* and sending readable files to fileread function THE OTHERS will be use for */
	DIR *dir;				             /* recurtion for if it is a new directory */
  	struct dirent *ent;
  	char *directs,*direct,**directors=NULL;
  	int i=0,k;
  	
  	if((dir = opendir(path))==NULL){
  		
  		
	  }  
	ent = readdir(dir);

	while(ent){
		if(ent->d_name[0]=='.'){
			ent = readdir(dir);	
			continue;
		}
		
		direct = (char*) malloc(sizeof(char)*(size_of(path)+size_of(ent->d_name)+1));
 		sprintf(direct,"%s/%s",path,ent->d_name);
	
 	          if(textorFile(ent->d_name,direct)==1){ /* Text File*/
	             	fileread(tr,file_head,direct,ent->d_name);	
		}
		else if(textorFile(ent->d_name,direct)==0){ /*dir file*/
		 
  		          directors = (char**) realloc(directors,sizeof(char*)*++i);
  		          directors[i-1]= (char*) malloc(sizeof(char)*(size_of(ent->d_name)+1));
  		          strcpy(directors[i-1],ent->d_name);
		}
		free(direct);
		ent = readdir(dir);	
	}
	closedir(dir);	
 	
	for(k=0;k<i;k++){
		
		directs = (char*) malloc(sizeof(char)*(size_of(directors[k])+2+size_of(path)));
		sprintf(directs,"%s/%s",path,directors[k]);
		lookforTexts(tr,file_head,directs);
		free(directors[k]);
		
	}
	free(directors);
	
}
void ascending_P(struct tree* tr,struct files* file,struct tree *head){ /* prints binary TREE ÝN ORDER OF ASCENDÝNG*/
	
	if(tr==NULL){
		printf("tree is null");
		return;
	}
	if(tr->left){
		ascending_P(tr->left,file,head);
	}
	print_word(head,file,tr->word);
	if(tr->right){
		ascending_P(tr->right,file,head);
	}
}
void descending_P(struct tree* tr,struct files* file,struct tree *head){/* prints binary TREE ÝN ORDER OF DESCENDÝNG*/
	
          if(tr==NULL){
		printf("tree is null");
		return;
	}
	if(tr->right){
		descending_P(tr->right,file,head);
	}
	print_word(head,file,tr->word);
	if(tr->left){
		descending_P(tr->left,file,head);
	}	
}
void normal_P(struct tree* tr,struct files* file_head,struct tree *head){/* prints binary TREE ÝN PRE-ORDER*/
	
	if(tr==NULL){
		printf("tree is null");
		return;
	}
	print_word(head,file_head,tr->word);
	if(tr->left){
		normal_P(tr->left,file_head,head);
	}
	
	if(tr->right){
		normal_P(tr->right,file_head,head);
	}		
}
void createlinks(struct tree *tree_head,struct files *file_head){ /* CREATES MAÝN HEAD OF LÝNKEDLÝSTS*/
	
	tree_head->word = NULL;
	tree_head->left = NULL;
	tree_head->right = NULL;
	file_head->file_name = NULL;
}
char* find_file_name(char* path){ /* FÝNDÝNG FÝLE NAME FROM A PATH*/
	int i,k=0;
	char *name ;
	for(i=size_of(path);-1<i;i--){
		if(path[i]=='/'){
			name = (char*) malloc(sizeof(char)*(size_of(path)-i+1));
			for(i++;i<size_of(path)+1;i++){
				
				name[k++]=path[i];
				
			}
			
			break;
		}
	}
	name[k+1]='\0';
	return name;
}
void printf_Func(struct tree *tr,char *mode,struct files *file){ /* PROCESSÝNG PRÝNT COMMAND AND SENDÝNG DATA TO RELATED FUNCTÝONS*/
	FILE *fout; 
	char modes[2][4] = {"ASC","DSC"};
	fout=fopen("output.txt","a");
	if(strcmp(mode,modes[0])==0){
		
		printf("PRINT TREE ASC\n");
		fprintf(fout,"PRINT TREE ASC\n");
		fclose(fout);
		ascending_P(tr,file,tr);
	}
	else if(strcmp(mode,modes[1])==0){
		
		printf("PRINT TREE DSC\n");
		fprintf(fout,"PRINT TREE DSC\n");
		fclose(fout);
		descending_P(tr,file,tr);
	}
	else{
		
		printf("PRINT TREE\n");
		fprintf(fout,"PRINT TREE\n");
		fclose(fout);
		normal_P(tr,file,tr);
		
	}
	
}
int menu(char *str){  /* gets information from input and choose where the input belong for the process*/
/* Print = 0 , search = 1, remove = 2 , add = 3 , wrong input = -1*/
	
	char context[4][10]={"PRINT","SEARCH","REMOVE","ADD"};
	int take = -1;
	if(strcmp(str,context[0])==0){
		take = 0;	
	}
	else if(strcmp(str,context[1])==0){
	          take = 1;
	}			
	else if(strcmp(str,context[2])==0){	
		take = 2;
	}
	else if(strcmp(str,context[3])==0){	
		take = 3;
	}
	else{	
	}
	return take;
}
int main(int argc,char *argv[]){
    	/* MAÝN FUNCTÝON JUST SENDÝNG DATA TO FUNCTÝONS ,TAKÝNG ARGV OF CONSOLE AND CREATÝNG LÝNKEDLÝSTS ALSO OPENS OUTPUT.TXT */      
	struct tree *tree_head = (struct tree*)malloc(sizeof(struct tree));
	struct files *file_head = (struct files*)malloc(sizeof(struct files));
	FILE *fout = fopen("output.txt","w");fclose(fout);			/*C:/Users/HTC/Desktop/diagram/sample_dir*/
	int i=0,mnum;
	char *path,*command_path,**command_line,*mode;
	createlinks(tree_head,file_head);
	path = argv[1];
	command_path = argv[2];
	lookforTexts(tree_head,file_head,path);	      	
	command_line=fileread2(command_path);
	while(command_line[i][0]!='\0'){
		
		mnum =(int) menu(command_line[i]);
		switch(mnum){
			case 0: 	/* PRINT PROCESS FUNCTION*/
			     	
				i =i+2;
				mode = command_line[i];
				if(strcmp(mode,"ASC")==0||strcmp(mode,"DSC")==0){
					printf_Func(tree_head,mode,file_head);
				}
				else{
					i--;
					printf_Func(tree_head,NULL,file_head);
				}
				
				
				break;	
			case 1:		/*SEARCH PROCESS FUNCTION*/
			     	i=i+1;
			     	fout = fopen("output.txt","a");
				mode = command_line[i];	
			     	printf("SEARCH\n");
			     	fprintf(fout,"SEARCH\n");fclose(fout);
			     	
			     	print_word(tree_head,file_head,make_Word_Lower(mode));
			     	
			     	
			     	break;
			case 2:    /*REMOVE function*/
 	 		          i=i+1;
 	 		          fout = fopen("output.txt","a");
 	 		          mode = command_line[i];
	  			printf("REMOVE %s\n",mode);
	  			fprintf(fout,"REMOVE %s\n",mode);fclose(fout);
	  			
				remove_func(tree_head,mode,file_head);
				break;
			case 3:    /*ADD TEXT function*/
 	 		          i= i+1;
 	 		          fout = fopen("output.txt","a");
 	 		          mode = command_line[i];
 	 		          printf("ADD %s\n",find_file_name(mode));
	  			fprintf(fout,"ADD %s\n",find_file_name(mode));fclose(fout);
	  					
 	 		          fileread(tree_head,file_head,mode,find_file_name(mode));
				break;      	
		}
		
		i++;
	}
	
	return 0;
}


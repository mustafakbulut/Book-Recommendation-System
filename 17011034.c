#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
float ortalama(int points[25][8],int user){ // pred formülü icin kullanicilarin ortalamalarini buldugum fonksiyon
	int i; 
	int div; // okudugu kitap sayisi
	float ort=0; 
	for(i=0;i<8;i++){
		if(points[user][i]>0){
			ort+=points[user][i];
			div++;
		}
	}
	ort=ort/div; // en son puanlarini toplayip okudugu kitap sayisina bölüyorum
	return ort; 
}
//k_users dizisinde kullanicilarin indexi var 0.indexte 0 degeri var onun karsiligi users dizisinde 'U1'
void selectionSort(float arr[], int n,int* k_users) { // en yüksek benzerligi bulabilmek icin ters selection sort kullandim. k=3 oldugu icin 3 kere en büyügü bulup dizinin basina 
	int i, j; // dongü degiskenleri					  // atiyorum			
	int max; // max elemanin indexini tuttugum degisken									  
	float temp;  // gecici degisken
    for (i = 0; i < 3; i++) { // 3 kere max elemani buluyorum
        max = i; 
        for (j = i+1; j < n; j++){
        	if (arr[j] > arr[max]){
            	max = j; 
  			}
		}  
		temp=k_users[max]; // 3 en yüksek benzerligi bulurken ayni zamanda hangi kullanicilar oldugunu da bulmak icin kullanicilar dizisinde de swap islemi yapiyorum
		k_users[max]=k_users[i]; 
		k_users[i]=temp;  
        temp=arr[max];
        arr[max]=arr[i];
        arr[i]=temp;
    } 
} 
float sim(int user1,int user2,int points[25][8]){ //sim fonksiyonunda user1 ve user2 nin similarity degerini buluyorum, points dizisinde kullanicilarin kitaplara verdigi puanlar var
	int i; // dongu degiskeni
	float ort1=0; // ilk kullanicinin ortalamasi
	float ort2=0; // ikinci kullanicinin ortalamasi
	float div=0; // ortalama hesaplarken kullandigim ortak okunan kitap sayisi degiskeni
	float value1; // Formülü yazarken kullandigim degiskenler ;
	float value2;
	float pay=0;
	float payda1=0;
	float payda2=0;
	float payda=0;
	for(i=0;i<8;i++){ // ortalama hesaplarken ikisinin de okudugu kitaplari aldim, bu yüzden similarity 1 cikan durumlar olabiliyor
		if(points[user1][i]>0 && points[user2][i]>0){
			div++;
			ort1=ort1+points[user1][i];
			ort2=ort2+points[user2][i];
		}
	}
	
	ort1=ort1/div;
	ort2=ort2/div;

	for(i=0;i<8;i++){  // ikisinin de okudugu kitaplari kullanarak formülü yazdim
		if(points[user1][i]>0 && points[user2][i]>0){
			pay=pay+((points[user1][i]-ort1)*(points[user2][i]-ort2)); // pay ve payda olarak ayirdim
			value1=points[user1][i]-ort1;
			value1=value1*value1;
			value2=points[user2][i]-ort2;
			value2=value2*value2;
			payda1=payda1+value1; 
			payda2=payda2+value2;
		}
	}
	payda1=sqrt(payda1); // formüldeki kök islemi
	payda2=sqrt(payda2);
	payda=payda1*payda2; 

	return pay/payda ;	// en son sim degerini döndürdüm
}
void pred(int points[25][8],char users[25][10],char books[8][100]){ // prediction fonksiyonu ,points=kullanicilarin kitaplara verdigi puanlar, users= kullanicilar, books= kitaplar
	int i,j,k; //dongü degiskenleri
	int* k_users; //sim hesaplarken kullanici indexlerini tuttugum gecici users dizisi
	float* sims;  // her NU kullanicisi icin en yüksek similarity degerlerini bulmak icin kullandigim sims dizisi
	float predict; // tahmin edilen puan degeri
	float ort; // ortalama
	float pay,payda; // formülde kullandigim pay ve payda degiskenleri
	float prevpredict; // en yüksek tahmin puani olan kitabi önerebilmek icin bir önceki puani tuttugum degisken
	char book[20]; // önerilen kitabi tuttugum dizi
	for(i=20;i<25;i++){ // NU1-NU5 kullanicilari users dizisinin 20-24 indexlerinde oldugu icin 20den 25e kadar gidiyorum ve her biri icin benzerlikleri bulup kitap öneriyorum
		k_users=(int*)calloc(20,sizeof(int));
		for(k=0;k<20;k++){
			k_users[k]=k;
		}
		sims=(float*)calloc(20,sizeof(float));
		system("COLOR");
		printf("\e[1;93m\n%s kullanicisi icin:\n\e[0m",users[i]);
		for(j=0;j<20;j++){
			sims[j]=sim(i,j,points); //NU kullanicilari icin U1-U20 arasindaki kullanicilar ile benzerlikleri
		}
		selectionSort(sims,20,k_users); // ters selection sort ile en yüksek 3 benzerligin bulunmasi, 3 kere en büyügü bulup basa atiyorum diziyi tamamen siralamiyorum
		printf("\nEn yakin kullanicilar:\n%s=%f \n%s=%f \n%s=%f\n",users[k_users[0]],sims[0],users[k_users[1]],sims[1],users[k_users[2]],sims[2]);
		prevpredict=0;
		for(j=0;j<8;j++){	
			if(points[i][j]==0){ //eger kullanici kitabi okumamissa verecegi tahmini puani pred formülü ile hesapliyorum	
				pay=0;
				payda=0;
				predict=0;
				ort=ortalama(points,i);
				predict+=ort;
				for(k=0;k<3;k++){  // formüldeki toplam islemini döngü olarak yapiyorum 3 tane en yakin kullanici oldugu icin
					pay=pay+sims[k]*(points[k_users[k]][j]-ortalama(points,k_users[k])); 
					payda=payda+sims[k];
				}
				predict+=pay/payda; 
				if(predict>prevpredict){ // bir onceki tahmin edilen puan ile karsilastiriyorum böylece en yüksek puan ile tahmin edilen kitabi öneriyorum
					strcpy(book,books[j]);
				}
				prevpredict=predict;
				
				printf("\n%s kullanicisinin %s kitabi icin tahmini puani= %f \n",users[i],books[j],predict);
			}
		}
		system("COLOR");
		//printf("\nOnerilen kitap: %s \n",book);
		printf("\n\e[1;94mOnerilen Kitap: %s \e[0m\n",book);
		printf("-----------------------------------------------------------------------");
	}	
}
int main(){
    FILE* fp; 
    char text[300]; // dosyayi okurken kullandigim string
    char* ptr; // dosyadan okudugum satiri tokenize ederken kullandigim pointer
    int i,j; // dongü degiskenleri
    char books[8][100]; // 8 tane kitabi tuttugum matris
    int points[25][8]; // tum kullanicilarin kitaplara verdigi puani tuttugum matris 
    char users[25][10]; // tum kullanicilarin isimlerini tuttugum matris
    fp=fopen("RecomendationDataSet.csv","r");
    if(fp==NULL){
        printf("Dosya acilamadi.");
    }
    fgets(text,200,fp);
    i=0;
    ptr=strtok(text,",");
    while(ptr != NULL){ // ilk olarak kitaplari dosyadan okuyorum
	    strcpy(books[i],ptr);
		i++;    
        ptr = strtok(NULL,",");
    }
	i=0;
	while(!feof(fp)){ // Kitaplardan sonra her satiri okuyorum ',' karakterine göre tokenize edip ilk stringi users matrisine diger degerleri de o kullanicinin puanlari olarak
		j=0;		  // points matrisine atiyorum
		fgets(text,200,fp);
		ptr=strtok(text,",");
		strcpy(users[i],ptr);
		ptr=strtok(NULL,",");
		while(ptr != NULL){
	   		if(strcmp(ptr," ")==0){ //csv dosyasinda okunmayan kitaplar icinn bosluk biraktim 
	   			points[i][j]=0;	
			}
			else{
				points[i][j]=atoi(ptr); // puanlar string tipinde oldugu icin integera atoi ile integera ceviriyorum       
			}
			ptr = strtok(NULL,",");	
      	  	j++;
    	}
		i++;	
	}
	pred(points,users,books); // en son bu matrisleri pred fonksiyonuna gönderiyorum
    return 0;
}

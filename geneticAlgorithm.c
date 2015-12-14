#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define POPULATION 30   //個体数
#define CODE_LENGTH 22 //遺伝子長
#define GENERATION 50  //計算する世代数
#define MAX 2.0        //表現が型が取り得る最大値
#define MIN -1.0       //表現型が取り得る最小値
#define MUTATE_PRO 0.03//突然変異率

//初期化
void gen_int_POPULATION(int old_pop[][CODE_LENGTH]);
//新しい世代の生成
void gen_population(int old_pop[][CODE_LENGTH],int new_pop[][CODE_LENGTH]);
//遺伝子型の表示
void disp(int pop[][CODE_LENGTH], int gen);
//表現型から適応度の計算
double fitness_function(double phenotypes[]);
//遺伝子型から適応度の計算
void calc_fitness(int pop[][CODE_LENGTH],double fitness[]);
//ルーレット選択
void roulette_strategy(int p_pop[][CODE_LENGTH],int p[]);
//
void crossover_1pt(int pa[],int pb[], int c[]);
//突然変異
void mutate(int c[]);
//２進数を10進数に変換する汎用関数
unsigned int binary2integer(int binary[],int bit_length);
//正数のべき乗関数
int powi(int m,int n);


//目的関数
double func(double x){
    return x * sin(10 * M_PI * x) + 2.0;
}

//プログラムのエントリポイント
int main(int argc, const char * argv[])
{
    int new_pop[POPULATION][CODE_LENGTH];
    int old_pop[POPULATION][CODE_LENGTH];
    
    //乱数の種の生成
    srand((unsigned)time(NULL));
    
    
    return 0;
}


//初期集団の生成
void gen_int_POPULATION(int old_pop[][CODE_LENGTH]){
    int i,j;
    
    for(i=0;i<POPULATION;i++){
        for(j=0;j<CODE_LENGTH;j++){
            //遺伝子のランダム生成
            old_pop[i][j] = rand() % 2;
        }
    }
}


//評価値の計算
void calc_fitness(int pop[][CODE_LENGTH],double fitness[]){
    unsigned int tmp;
    int binary[CODE_LENGTH];
    double phenotype;

    //全個体の適合度計算
    for(int i = 0; i<POPULATION; i++){
        //デコード
        tmp = binary2integer(pop[i],CODE_LENGTH);
        phenotype = 1.0 + tmp * 3.0 / (pow(2.0,22) - 1);
        fitness[i] = func(phenotype);
    }
}

//べき乗関数
int powi(int m,int n){
    int prod = 1;
    
    if(n==0){
        return 1;
    }
    else if(n>0){
        for (int i=0;i<n;i++){
            prod*=m;
        }
    }
    else {
        fprintf(stderr,"Error : powi()\n");
    }
}

//次世代の生成（ルーレット選択）
void gen_population(int old_pop[][CODE_LENGTH],int new_pop[][CODE_LENGTH]){
    //TODO
}

//ルーレット選択
void roulette_strategy(int p_pop[][CODE_LENGTH],int p[]){
    double fit[POPULATION];
    double p_fit[POPULATION];
    double sum_fit = 0.0;
    double tmp_fit = 0.0;

    calc_fitness(p_pop,fit);
    //各個体の選択確率を計算
    for(int i=0;i<POPULATION;i++){
        sum_fit += fit[i];
    }
    for(int i=0;i<POPULATION;i++){
        p_fit[i] = fit[i]/sum_fit;
    }
    //ルーレット選択
    double r = (double)rand()/(RAND_MAX+1.0);
    for(int i=0;i<POPULATION;i++){
        //選択確率の計算
        tmp_fit += p_fit[i];
        if(tmp_fit>r){
            //個体を取り出す
            for(int j = 0; j < CODE_LENGTH;j++){
                p[j]= p_pop[i][j];
            }
            break;
        }
    }
}
    

//２進数を10進数に変換
unsigned int binary2integer(int binary[],int bit_length){
    unsigned int integer=0;
    for(int i=bit_length - 1; i >= 0; i--){
        integer += binary[i] * powi(2,i);
    }
    return integer;
}

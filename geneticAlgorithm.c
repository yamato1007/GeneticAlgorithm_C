#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define POPULATION 1000   //個体数
#define CODE_LENGTH 22 //遺伝子長
#define GENERATION 50  //計算する世代数
#define MAX 2.0        //表現が型が取り得る最大値
#define MIN -1.0       //表現型が取り得る最小値
#define MUTATE_PRO 0.03//突然変異率

//初期化
void gen_int_population(int old_pop[][CODE_LENGTH]);
//新しい世代の生成
void gen_population(int old_pop[][CODE_LENGTH],int new_pop[][CODE_LENGTH]);
//遺伝子型から適応度の計算
void calc_fitness(int pop[][CODE_LENGTH],double fitness[]);
//ルーレット選択
void roulette_strategy(int p_pop[][CODE_LENGTH],int p[]);
//一転交叉
void crossover_1pt(int pa[],int pb[], int c[]);
//突然変異
void mutate(int c[]);
//２進数を10進数に変換する汎用関数
unsigned int binary2integer(int binary[],int bit_length);
//正数のべき乗関数
int powi(int m,int n);
//遺伝子型の表示
void disp(int pop[][CODE_LENGTH]);


//目的関数
double func(double x){
    return x * sin(10 * M_PI * x) + 2.0;
}

//全部表示
void dispall(int pop[][CODE_LENGTH]){
    
    double fitness[POPULATION];
    calc_fitness(pop,fitness);
    for(int i=0;i<POPULATION;i++){
        for(int j=0;j<CODE_LENGTH;j++){
            printf("%d",pop[i][j]);
        }
        printf(" : %lf",fitness[i]);
        printf("\n");
    }
    printf("\n");
}

//遺伝子型のコピー
void copy(int pop1[][CODE_LENGTH],int pop2[][CODE_LENGTH]){
    for(int i=0;i<POPULATION;i++){
        for(int j=0;j<CODE_LENGTH;j++){
            pop2[i][j] = pop1[i][j];
        }
    }
}


//プログラムのエントリポイント
int main(int argc, const char * argv[])
{
    int new_pop[POPULATION][CODE_LENGTH];
    int old_pop[POPULATION][CODE_LENGTH];
    
    //乱数の種の生成
    srand((unsigned)time(NULL));

    //初期集団の生成
    gen_int_population(old_pop);
    
    for(int i=0;i<GENERATION;i++){
//        dispall(old_pop);
        disp(old_pop);
        //次世代の生成
        gen_population(old_pop,new_pop);
        copy(new_pop,old_pop);
    }


    return 0;
}


//初期集団の生成
void gen_int_population(int old_pop[][CODE_LENGTH]){
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
        phenotype = -1.0 + tmp * 3.0 / (pow(2.0,22) - 1);
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
    return prod;
}

//次世代の生成（ルーレット選択）
void gen_population(int old_pop[][CODE_LENGTH],int new_pop[][CODE_LENGTH]){
    for(int i=0;i<POPULATION;i++){
        int parent[2][CODE_LENGTH];
        for(int i=0;i<2;i++){
            roulette_strategy(old_pop,parent[i]);
        }
        int child[CODE_LENGTH];
        crossover_1pt(parent[0],parent[1],child);
        mutate(child);
        for(int j=0;j<CODE_LENGTH;j++){
            new_pop[i][j] = child[j];
        }
    }
}

//ルーレット選択
void roulette_strategy(int p_pop[][CODE_LENGTH],int p[]){
    double fit[POPULATION];
    double p_fit[POPULATION];
    double sum_fit = 0.0;

    //全個体の適合度計算
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
    double tmp_fit = 0.0;
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
    
//一転交叉
void crossover_1pt(int pa[],int pb[], int c[]){
    int cross_point;
    
    cross_point = (int)(rand()/(RAND_MAX+1.0)*CODE_LENGTH);
    //一転交叉
    int i;
    for(i=0;i<cross_point;i++){
        c[i]=pa[i];
    }
    for( ; i < CODE_LENGTH ;i++){
        c[i] = pa[i];
    }
}

//突然変異
void mutate(int c[]){
    double r;
    for(int i=0; i<CODE_LENGTH; i++){
        r = (double)rand()/(RAND_MAX+1.0);
        if(r < MUTATE_PRO){
            c[i] = (c[i] + 1) % 2;
        }
    }
}

//遺伝子型の表示
void disp(int pop[][CODE_LENGTH]){
    double max=0.0;
    double sum=0.0;
    double ave;
    double phenotype;
    double fitness[POPULATION];


    calc_fitness(pop,fitness);
    
    int j = 0;
    for(int i=0;i<POPULATION;i++){
        if(max < fitness[j]){
            j = i;
            max = fitness[i];
        }
        sum += fitness[i];
    }
    ave = sum / POPULATION;

    //デコード
    int tmp = binary2integer(pop[j],CODE_LENGTH);
    phenotype = -1.0+tmp*3.0/(pow(2.0,22)-1.0);

    //計算結果の表示
    printf("%lf %lf\n", phenotype,max);
}



//２進数を10進数に変換
unsigned int binary2integer(int binary[],int bit_length){
    unsigned int integer=0;
    for(int i=bit_length - 1; i >= 0; i--){
        integer += binary[i] * powi(2,i);
    }
    return integer;
}

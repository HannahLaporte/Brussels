#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<math.h>
#define bool int
#define false 0
#define true 1
#define FeatureMax 20  //宏定义特征数最大值，好改数据
#define DataMax 600    //样本数目（几条）最大
#define TrainNum 9999      //训练次数
#define LearnV 0.04         //学习率


double data[DataMax][FeatureMax]; //存储房价数据集的数组
double max[FeatureMax];   //存最大值用于数据归一化
double min[FeatureMax];   //存最小值用于数据归一化
int dataNum; //样本数目
int trdataNum;  //训练样本数目
int featureNum;//特征数目
int n;     //字段数(featureNum+1)

double sumXY[FeatureMax];//x*y的求和
double sumX[FeatureMax]; //x的求和
double sumY;//y的求和
double everageX[FeatureMax];  //x平均
double everageY; //y平均
double varianceX[FeatureMax]; //x方差
double varianceY; //y方差
double recoef[FeatureMax];  //各特征皮尔逊相关系数

int p;   //要选择最相关特征数（比如题中说的4）
int pos[FeatureMax];   //记录选择的那几个特征在原数据集的列数
double datase[DataMax][FeatureMax]; //参与线性方程的数据集，挑出来的
double coef[TrainNum+1][FeatureMax];  //系数矩阵,最后一行是训练结束的系数结果

double rmse;  //测试集中的均方根误差

double realcoef[FeatureMax];  //真正的系数，逆归一化后的

struct Feature {
	char* name[FeatureMax];//特征名字字符串
	double coef[FeatureMax];  //特征系数
};
struct Feature Feain, Feaout;  //存的特征和选择的特征

bool Inputs();
void Init();
void SortSelect();
void LineReg();
void Text();
void ReInit();
void Outputs();

int main() {
	if (Inputs() == false) return 0;
//	for (int i = 0; i < dataNum; i++) {
//		for (int j = 1; j < n; j++) {
//			printf("%lf",data[i][j]);
//		}
//	}
	Init();
	SortSelect();
	LineReg();
	Text();
	ReInit();
	Outputs();
	return 0;
}

//数据读入
bool Inputs() {
	int i;
	for (i = 0; i < FeatureMax; i++) {
		Feain.name[i] = (char*)malloc(sizeof(char) * 10);
		Feaout.name[i] = (char*)malloc(sizeof(char) * 10);
	}

	//char fname[256];//文件名
	//printf("请输入存放数据的文件名： ");
	//scanf("%s", fname);
	printf("存放数据的文件名：housing.txt\n");
	printf("\n样本数目dataNum：\n");   //506
	scanf("%d", &dataNum);
	printf("\n特征数目featureNum:\n");  //算上房价 14
	scanf("%d", &featureNum);
	n = featureNum + 1;
	trdataNum = dataNum * 7 / 10;   //样本中训练样本数（数据集划分!!!

	FILE* fp = fopen("housing.txt", "rb");//为了每次运行不用重新输入名字
	if (fp == NULL) {
		printf("不能打开输入的文件\n");
		fprintf(stderr, "open file error: %s", strerror(errno));  //错误原因
		return false;
	}

	Feain.name[0] = '\0';
	Feaout.name[0] = '\0';
	for (i = 1; i < n; i++) {
		fscanf(fp, "%s", Feain.name[i]);
	}

	memset(max, 0, sizeof(max));//初始化最大数组都为0
	memset(min, 0x7f7f7f7f, sizeof(min));//无穷大
	for ( i = 0; i < dataNum; i++) {
		data[i][0] = 1; //  令初始系数为1，x0是1
		for (int j = 1; j < n; j++) {
			fscanf(fp, "%lf", &data[i][j]);    //存数据的同时找各特征最大最小值
			if (data[i][j] > max[j])
				max[j] = data[i][j];
			if (data[i][j] < min[j])
				min[j] = data[i][j];

		}
	}
	fclose(fp);

	return true;
}

//初始化  数据归一化
void Init() {
	for (int i = 0; i < dataNum; i++) {
		for (int j = 1; j < n; j++) {
			data[i][j] = 1.0*(data[i][j] - min[j]) / (max[j] - min[j]);
		}
	}
}

//排序选择各特征的相关系数！！！选几个最相关维度！
void SortSelect() {
	int i, j;
	sumY = 0;
	for (i = 0; i < dataNum; i++) {
		sumY += data[i][featureNum];

	}
	//printf("\n sumY=%f", sumY);
	for (j = 1;j < featureNum; j++) {
		sumX[j] = 0;
		sumXY[j] = 0;
		for (i = 0; i < dataNum; i++) {
			sumX[j] += data[i][j];
			sumXY[j] += data[i][j] * data[i][featureNum];
		}
		//printf("\n sumX[%d]=%f", j,sumX[j]);
		//printf("\n sumXY[%d]=%f", j, sumXY[j]);
	}

	everageY = 1.0*sumY / dataNum;
	//printf("\n everageY=%f", everageY);
	double t = 0;
	for (i = 0; i < dataNum; i++) {
		t += pow(data[i][featureNum] - everageY, 2);
	}
	varianceY = 1.0*t / dataNum;
	//printf("\n varianceY=%f", varianceY);
	for (j = 1; j < featureNum; j++) {
		everageX[j] = 1.0*sumX[j] / dataNum;
		t = 0;
		for (i = 0; i < dataNum; i++) {
			t += pow(data[i][j] - everageX[j], 2);
		}
		varianceX[j] = 1.0*t / dataNum;
		//printf("\n varianceX[%d]=%f", j, varianceX[j]);
	}
	printf("\n各特征相关系数：\n");
	for (j = 1; j < featureNum; j++) {
		recoef[j] = 1.0*(1.0*sumXY[j] / dataNum - everageX[j] * everageY) / sqrt(varianceX[j] * varianceY);
		//printf("\n recoef[%d]=%f", j, recoef[j]);
		printf("%s\t%f\n", Feain.name[j], recoef[j]);
	}
	//因为有负数的系数，还有选择需要，建temp数组；
	double temp[FeatureMax];
	for (j = 1; j < featureNum; j++) {
		temp[j] = fabs(recoef[j]);
	}
	printf("\n请输入选择最相关特征的个数（1<=p<featureNum）\n");
	scanf_s("%d", &p);
	int k=1;
	int m = 1;
	while (k <= p) {
		int  max = 1;
		for (j = 1; j < featureNum; j++) {
			if (temp[j] > temp[max]) {
				max = j;
			}
		}
		temp[max] = 0;   //变0不参与比较了
		pos[m++] = max;
		strcpy(Feaout.name[k], Feain.name[max]);
		for (i = 0; i < dataNum; i++) {
			datase[i][k] = data[i][max];
		}
		k++;
	}
	for (i = 0; i < dataNum; i++) {    //不要忘了还有房价那一列
		datase[i][0] = 1;
		datase[i][p+1] = data[i][featureNum];
	}
//	for (i = 0; i < dataNum; i++) {
//		for (j = 0; j <=(p + 1); j++) {
//			printf("%f ", datase[i][j]);
//		}
//		printf("\n");
//	}
	//选的特征就构成一个新的数据集了
}

//线性回归  梯度下降 训练1000次！！ 求线性方程
void LineReg() {
	int i,j,k,l;
	//for (i = 0; i < TrainNum; i++) { //系数初始化为1
		for (j = 0; j < p+1; j++) {
			coef[0][j] = 1;
		}
	//}
	for (i = 1; i <=TrainNum; i++) {
		for (j = 0; j < p+1; j++) {
			double t1=0;
			for (l = 0; l < trdataNum; l++) {
				double t2 = 0;
				for (k = 0; k < p+1; k++) {
					t2 += (coef[i - 1][k] * datase[l][k]);

				}
				t1 += ((t2  - datase[l][p+1])* datase[l][j]);
			}
			coef[i][j] = coef[i-1][j]-(1.0*LearnV*t1/trdataNum);
			//printf("\ncoef[%d][%d]=%f\n", i, j, coef[i][j]);
		}

	}

	printf("\n训练%d次后，线性方程中相应的系数(未反归一化)：\n",TrainNum,p);
	for (i = 0; i < p + 1; i++) {
		printf("%10s : %10f\n", Feaout.name[i], coef[TrainNum][i]);
	}
}

void Text() {
	int i, j;
	double t1 = 0;
	for (i = trdataNum; i < dataNum; i++) {
		double t2 = 0;
		for (j = 0; j < p+1; j++) {
			t2 += coef[TrainNum][j] * datase[i][j];
		}
		t2 =pow(t2-datase[i][p+1],2);
		t1 += t2;
	}
	//printf("\n t1=%f \n", t1);
	double num = (double)dataNum - trdataNum;
	rmse = sqrt(1.0*t1 / num);
	printf("\n训练%d次后的RMSE为：%f\n",TrainNum,rmse);
}
//系数反归一化
void ReInit() {
	int i;
	for (i = 0; i < p + 1; i++) {
		realcoef[i] = coef[TrainNum][i];
	}
	for (i = 1; i <= p; i++) {//先计算w0的真实值
		realcoef[0] -= (1.0 * coef[TrainNum][i] * min[pos[i]] / (max[pos[i]] - min[pos[i]]));
	}
	realcoef[0] = realcoef[0] * (max[featureNum] - min[featureNum]) + min[featureNum];
	int j;
	for (j = 1; j < p+1; j++) {
		realcoef[j] *= (1.0 * (max[featureNum] - min[featureNum]) / (max[pos[j]] - min[pos[j]]));
	}
	printf("\n训练%d次后，线性方程中相应的系数(已反归一化)：\n",TrainNum, p);
	for (i = 0; i < p + 1; i++) {
		printf("%10s : %10f\n", Feaout.name[i], realcoef[i]);
	}
}

//输出
void Outputs() {
	printf("\n接下来可进行房价预测\n");
	int i;
	double x[FeatureMax];
	x[0] = 1;
	for (i = 1; i < p + 1; i++) {
		printf("请输入%s的值:\n", Feaout.name[i]);
		scanf_s("%lf", &x[i]);
	}
	double result=0;
	for (i = 0; i < p + 1;i++) {
		result += realcoef[i] * x[i];
	}
	printf("预测结果为：%f", result);
}


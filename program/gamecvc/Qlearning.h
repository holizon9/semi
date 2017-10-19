/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */
/** \mainpage Q学習サンプル
 **
 **		\author Shuichi ARAI (arai.s@cs.tcu.ac.jp)
 **		\version 1.2
 **		\date 2010-2014
 **
 **		\section intro Introduction
 **		このプログラム群は千葉工業大学 情報工学科の「ロボット工学」で利用するQ学習のサンプルプログラムです．\n
 **		これらのプログラムでは以下の2つのライブラリ(Q学習ライブラリ，Gnuplot表示ライブラリ)を利用します．\n
 **		
 **		- \subpage libQlearn
 **		- \subpage libPlot
 **
 **		サンプルプログラムは３種類用意しました．\n
 **		- 迷路をどのように進めばゴールに効率的にたどり着けるか(経路探索)を学習する
 **		- ○×ゲームでどこに打てば勝てるかを学習する
 **		- 移動ロボットの関節をどのように動かせば効率的に前進できるかを学習する
 **
 **	\section sec1 本サンプルプログラム群のコンパイルの仕方
 **		-#	ライブラリの作成
 **			-	./lib/src/ directoryに移動します．
 **			-	そのdirectoryにMakefileがあるので， make します．
 **			-	./lib/ directoryに libQlearn.a というファイルができていればOKです．
 **		-#	経路探索プログラムの作成
 **			-	./Maze/ directoryに移動します．
 **			-	そのdirectoryにMakefileがあるので， make します．
 **			-	maze という実行ファイルができていればOKです．
 **		-#	○×プログラムの作成
 **			-	./TicTacToe/ directoryに移動します．
 **			-	そのdirectoryにMakefileがあるので， make します．
 **			-	learn_play_TicTacToe, play_TicTacToe, showQtable という3つの実行ファイルができていればOKです．
 **		-#	移動ロボットプログラムの作成
 **			-	./Robot/ directoryに移動します．
 **			-	そのdirectoryにMakefileがあるので， make します．
 **				robot という実行ファイルができていればOKです．
 **
 **	\section sec2 [Sample1]: 経路探索
 **		設定した迷路上を探索し，ゴールまでの最短経路をQ学習を用いて学習します．
 **		- \subpage maze
 **
 **	\section sec3 [Sample2]: Tic-Tac-Toe
 **		○×ゲームの解を探索します．
 **		- \subpage TicTacToe
 **
 **	\section sec4 [Sample3]: Robotの動作の学習
 **		2関節移動ロボットの前進動作を学習します．
 **		- \subpage Robot
 **
 **	\section sec5 演習1
 **		-# まず，全てのプログラムをコンパイルし，実行モジュールを作成しましょう．
 **		-# 各プログラムを実際に動作させてみましょう．
 **/
/* _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/ */
/**
 ** \page libQlearn Q学習ライブラリ
 **
 **	Q学習を用いたプログラムを容易に作成できるよう，次のデータ構造と関数群を用意しました．
 **		- データ構造
 **			- ::QTable
 **		- 関数群
 **			- initQtable()			:	Qテーブルの確保
 **			- Q_Learning()			:	現状態で行動を実行した際に得られる報酬を元に学習を行う
 **			- selectAction()		:	行動選択
 **			- maxQval()				:	状態(state)での最大Q値を得る
 **			- epsilonGreedy()		:	Epsilon Greedy法
 **			- boltzmannSelection()	: Boltzmann Action Selection法
 **			- saveQtable()			: Q-tableの保存
 **			- loadQtable()			: Q-tableの読み込み
 **			- showQtable()			: Qテーブルの表示
 **
 **/

#ifndef _Q_LEARNING_H_
#define _Q_LEARNING_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

//#define NUM_TRIAL		300		/*!< 試行回数 */
//#define NUM_TRIAL		1000		/*!< 試行回数 */
#define NUM_TRIAL		100000		/*!< 試行回数 */
//#define NUM_TRIAL		1000000		/*!< 試行回数 */
//#define NUM_TRIAL		10000000		/*!< 試行回数 */
#define NUM_MAX_STEP	9		/*!< 最大Step数 (1試行におけるQ値の更新回数) */

#define Q_LEARNING_ALPHA	0.5	/*!< Q-learning 学習率 */
#define Q_LEARNING_GAMMA	0.9	/*!< Q-learning 割引率 */
#define Q_LEARNING_EPSILON	10	/*!< ε-greedy法でのε(%) */


/** Q-table **/
typedef struct{
	int 		numStates;	/*!< 状態数 */
	int			numActions;	/*!< 行動種類数 */
	double** 	Q;			/*!< Q値 */
	int*		cntLearn;	/*!< 学習回数 */
	double		alpha;		/*!< 学習率 (0<α<=1) */
	double		gamma;		/*!< 割引率 (0<=γ<1) */
}QTable;


/*** Function Prototypes ***/

/** Qテーブルの確保
 **
 **	\param 	numStates	:	状態数
 **	\param	numActions	:	行動数
 **	\param	alpha		:	学習率 (0<α<=1)
 **	\param	gamma		:	割引率 (0<=γ<1)
 **
 **	\return Q-table
 **/
QTable* initQtable(int numStates, int numActions, double alpha, double gamma);


/**	現状態で行動を実行した際に得られる報酬を元に学習を行う
 **
 **		時刻\f$ t \f$において，エージェントが状態\f$s_t\f$にいるとき，
 **		エージェントの行動選択法に従って，行動\f$a_t\f$を選択したとする．\n
 **		その結果，エージェントは環境から報酬\f$r_t\f$を得たとき，Q学習は以下の式で行われる．
 **		\f[ Q(s_t, a_t) = (1-\alpha)Q(s_t, a_t) + \alpha(r_t + \gamma \max_{a} Q(s_{t+1}, a) ) \f]
 **		ただし，\f$\alpha, \gamma\f$はそれぞれ，学習率と割引率であり，\f$ 0 < \alpha, \gamma \le 1 \f$
 **		の値を持つ．
 ** 
 **	\param	curState	:	現状態
 **	\param	action		:	行動
 **	\param	nextState	:	行動(action)を実行した後の状態
 **	\param	reward		:	行動(action)を実行した後に得られる報酬
 **	\param	Qtable		:	Q-table
 **/
void Q_Learning(int curState, int action, int nextState, int reward, QTable* Qtable);


/** 行動選択
 **		指定した状態(state)におけるQ値が最大の行動を選択する．\n
 **		ただし，
 **		状態stateでのQ値最大の行動が複数ある場合は，最大Q値を持つ行動の中からランダムに選択する．
 **
 **	\param state	:	状態
 **	\param Qtable	:	Q-table
 **
 **	\return Q値が最大の行動番号
 **/
int selectAction(int state, QTable* Qtable);


/** 状態(state)での最大Q値を探索する
 **
 **	\param	state	:	状態
 **	\param	Qtable	:	Qテーブル
 **
 **	\return 最大Q値
 **/
double maxQval(int state, QTable* Qtable);


/**	Epsilon Greedy法
 **
 **	Epsilon Greedy法は，行動選択法の1つである．
 ** 一般に，ある状態において最適な選択はQ値が最大になる行動を選択することである．
 ** しかし，最大Q値の行動を選択するのみだと，局所解に陥り最適な解を探索することができなるなる．
 **	そこで，Epsilon Greedy法では，予め定めた一定確率(ε)の割合でランダムに行動を選択する．
 **
 **	\param state	:	状態
 **	\param Qtable	:	Q-table
 **	\param epsilon	:	ε(%)
 **
 **	\return 行動番号
 **/
int epsilonGreedy(int state, QTable* Qtable, int epsilon);

/** Boltzmann Action Selection法
 **
 **		時刻\f$ t \f$において，エージェントが状態\f$s_t\f$にいるとき，
 **		その状態で取りうる行動\f$a_t\f$毎にQ値(\f$Q(s_t, a_t)\f$)を持つ．\n
 **		一般的には，このQ値が最大となる行動を選択するのだが，それを繰り返すと局所解に陥り
 **		最適な解を探索することができなくなる．\n
 **		そこで，気体分子がとり得るエネルギー準位による分子の様子を表現したボルツマン分布を用いて，
 **		行動選択を行うソフトマックス的手法が提案されている．\n
 **		ボルツマン分布に従い，ある状態\f$s_t\f$である行動\f$a_t\f$を取る確率\f$p(a_t|s_t)\f$は以下のように書ける．
 **		\f[ p(a_t|s_t) = \frac{ \exp(Q(s_t, a_t))/T }{\sum_a \exp(Q(s_t, a)/T)} \f]
 **		ただし，\f$T\f$は温度定数であり，学習と共に\f$T\f$を小さくしていくのが一般的である．
 **
 **		この確率が大きいほど，尤もらしい行動であるので，この確率に応じたルーレット選択を行い，行動を選択する．
 ** 
 **	
 **	\param state    :   状態
 ** \param Qtable   :   Q-table
 ** \param T        :   Temperature
 **
 ** \return 行動番号
 **/
int boltzmannSelection(int state, QTable* Qtable, double T);


/**	Q-tableの表示
 **
 **	\param Qtable	:	Q-table
 **/
void showQtable(QTable* Qtable);

/** Q-tableのファイルへの保存
 **
 ** \param Qtable   :   Q-table
 ** \param filename : Q-table filename
 **/
void saveQtable(QTable* Qtable, char* filename);

/** Q-tableファイルの読み込み
 **
 ** \param filename : Q-table filename
 **
 **	\return Q-table
 **/
QTable* loadQtable(char* filename);



#endif	/* _Q_LEARNING_H_ */

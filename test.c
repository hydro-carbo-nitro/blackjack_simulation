#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* initial condition */
int N = 15;     // 전체 플레이어 수
int p = 30;     // 카드를 뽑을 확률 [0 ~ 100(%)]
int limit = 16; // 딜러가 게임을 정지할 조건
int entire_deck[13] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};      // 뽑을 수 있는 카드의 종류

int rand_deck;  // 덱에서 어떤 카드를 뽑을지 결정하는 난수 (entire_deck과 연관있음)
int rand_draw;  // 카드를 뽑을지 말지 결정하는 난수 (p와 연관있음)
int drawn_card; // 뽑힌 카드 (entire_deck과 연관있음)


void output_value(int **players_arr, int *count_arr)
/* 이중 포인터로 생성된 배열을 확인하는 함수 */
{
    for (int i = 0; i < N; i++) 
    {
        printf("%dth Player : [ ", i);
        for (int j = 0; j < count_arr[i]; j++)
        {
            printf("%d ", players_arr[i][j]);
        }
        printf("]\n");
    }
}

void draw_card(int **player_arr, int *count_arr, int *sum_arr, int index)
{
    int individual_cardcount = count_arr[index];        // 현재 갖고있는 카드의 매수 (임시)
    int individual_sum = sum_arr[index];                // 현재 갖고있는 카드의 합 (임시)

    rand_deck = rand()%13;                              // 어떤 카드를 뽑을지 (전역변수, 임시)
    drawn_card = entire_deck[rand_deck];                       // 뽑은 카드 (전역변수, 임시)

    if (individual_sum > 21);
    {

    }
}

int main()
{
    srand(time(NULL)); // 난수 초기화

    /* 동적할당 선언 */
    int *count_of_deck;
    count_of_deck = (int *)malloc(sizeof(int *) * N);     // 덱 숫자에 대한 정보가 담겨있는 arr
    for (int i = 0; i < N; i++)
    {
        count_of_deck[i] = 0;
    }

    int *sum_of_deck;
    sum_of_deck = (int *)malloc(sizeof(int *) * N);     // 카드의 합에 대한 정보가 담겨있는 arr
    for (int i = 0; i < N; i++)
    {
        sum_of_deck[i] = 0;
    }

    int **players;
    players = (int **)malloc(sizeof(int *) * N);  // 배열의 row. N명의 자리
    for (int i = 0; i < N; i++)
    {
        players[i] = (int *)malloc(sizeof(int) * 2);   // 배열의 col. 2장 먼저 받고 시작
        for (int j = 0; j < 2; j++)
        {
            rand_deck = rand()%13;                              // 어떤 카드를 뽑을지 (전역변수, 임시)
            drawn_card = entire_deck[rand_deck];                // 뽑은 카드 (전역변수, 임시)
            
            players[i][j] = drawn_card;                         // 카드 뽑았으니 덱에 넣기
            count_of_deck[i]++;                                 // 카드 매수 늘리기
            sum_of_deck[i] += drawn_card;                       // 카드의 합 늘리기

            if (sum_of_deck[i] > 21)
            {
                /* 첫 두번의 드로우에서 21이 넘는 경우는 A 두장, 즉 11 + 11 뿐이므로
                나중에 뽑힌 11을 1로 만들어주면 된다. */
                players[i][j] = 1;      
                sum_of_deck[i] -= 10;
            }
        }
    }
    /* 카드 두장 나눠주기 완료 */
    printf("-------------------Initial Deck-------------------\n");
    output_value(players, count_of_deck);  // input된 값 output
    printf("--------------------------------------------------\n");

    /* 그 이후 진행 */

    /* 종료 */

    /* 동적할당 해제 */
    for (int i = 0; i < N; i++)
    {
        free(players[i]);
    }
    
    free(players);
    free(count_of_deck);
    free(sum_of_deck);

    return 0;

}
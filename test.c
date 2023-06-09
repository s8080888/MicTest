#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <windows.h>

// 子執行緒函數
void* child(void* data) {
  char *str = (char*) data; // 取得輸入資料
  for(int i = 1;i > 0;++i) {
    Sleep(10);
  }
  pthread_exit(NULL); // 離開子執行緒
}

// 主程式
int main() {
  pthread_t t; // 宣告 pthread 變數
  pthread_create(&t, NULL, child, "Child"); // 建立子執行緒

  // 主執行緒工作
  for(int i = 0;i < 3;++i) {
    printf("Master\n"); // 每秒輸出文字
    Sleep(10);
  }

  pthread_join(t, NULL); // 等待子執行緒執行完成
  printf("Test");
  return 0;
}
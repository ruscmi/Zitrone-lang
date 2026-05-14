// ИМПОРТ ЛИБ
#include <stdio.h> // работа ввода вывода
#include <string.h> // для работы со строками
#include <stdbool.h> // для флагов

bool active = true; // флаг 
bool found_f_name = false; // флаг для того что бы искать переменные в памяти
int main() {
    // мой основной буфер
    char input[ 100 ] = { 0 } ;
    printf( "          == ZITRONE INTERPRETER ==        \n") ;
    printf( "HELLO GUYS! I LOVE LEMONS AND INTERPRETERS!\n") ;
    // переменная для разделения строки
    char *command;
    // переменные для запоминания значений
    char *name; // ключ
    char *val; // значение
    char *f_name; // проходим по значениям в druck(print)
    // структура для хеш-таблицы
    struct Variable{
        char name[50];
    	char val[50];
    };
    // массив памяти для наших переменных
    struct Variable memory[100] = {0};
    int count_variab = 0;
    // запуск цикла
    while( active ) {
        //ввод в буфер
		printf("\ninp or 'exit': ") ;
        // если строка ничего не возвращает мы выходим
		if( fgets ( input , 100 , stdin ) == NULL ) break ;
        //подмениваем \n на \0
		input[ strcspn ( input , "\n" ) ] = '\0' ;
		// режем строку как колбаску
		command = strtok(input, " "); // для первого разреза
        //команда выхода
		if( strcmp ( command , "exit" ) == 0 ) {
		    printf( "goodbyelmn\n" ) ;
		    active = false ; // при выходе меняю флаг
			break ;
		} // тут команда сета
		else if( strcmp (command , "erstellen" ) == 0 ) {
		    name = strtok(NULL," "); //ключ
		    val = strtok(NULL," "); //значение 
		    strcpy(memory[count_variab].name,name); // запоминаем в память ключ
		    strcpy(memory[count_variab].val,val); // запоминаем значение
		    count_variab++; // для того что бы точно сработало
            printf("%s = %s\n",name,val);
			continue ;
		}
		else if( strcmp (command , "druck") == 0) {
            f_name = strtok(NULL," "); // проверяем значение после druck(сам druck - command,все что после command - NULL)
	        while(f_name != NULL ) { // проходимся по всему f_name   
	           found_f_name = false;
	   		    for(int i = 0; i  < count_variab; i++ ) {
					if(strcmp(memory[i].name,f_name) == 0) { // если находим выводим
				        printf("%s",memory[i].val);
					    found_f_name = true; // ставим found_f_name в активное состояние для поиска
						break;
					}
				}
				if(!found_f_name) { // если нет выводим то что было напечатано в f_name
					printf("%s",f_name);
				}
				f_name  = strtok(NULL, " "); // попка три попки четыре попки
			}
		}
		else {  // если вы не вводите команду вам выдает ошибку.
			printf( " Error: dont have %s \n" ,command ) ;
		}
	}
}
// команды - druck - вывод, erstellen - создать

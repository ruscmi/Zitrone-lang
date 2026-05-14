// ИМПОРТ ЛИБ
#include <stdio.h> // работа ввода вывода
#include <string.h> // для работы со строками
#include <stdbool.h> // для флагов
#include <stdlib.h> // для atof и т.п.

bool active = true; // флаг 
bool found_f_name = false; // флаг для того что бы искать переменные в памяти
int main() {
    // мой основной буфер
    char input[ 100 ] = { 0 } ;
    printf( "          == ZITRONE INTERPRETER ==        \n") ;
    printf( "HELLO GUYS! I LOVE LEMONS AND INTERPRETERS!\n") ;
    // переменная для разделения строки
    char *command;
    double res; // переменная для вывода результата
    char *a_name; // первое число(строка)
    char op; // операция
    char *op_name; // значение для операции
    char *b_name; // второе число(строка,но потом мы с помощью double возводим число)
    // переменные для запоминания значений
    char *name; // ключ
    char *val; // значение
    char *f_name; // проходим по значениям в druck(print)
    // структура для хеш-таблицы
    struct Variable{
        char name[50];
    	char val[50];
    };
    int w_ind = -1;
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
		else if(strcmp(command, "berech") == 0) {
		    res = 0; // пустая переменная для вывода результата
			a_name = strtok(NULL, " "); // это будет 1 число
			op_name = strtok(NULL," "); // это будет операция
			op = op_name[0];
			b_name = strtok(NULL," "); // второе число
			if(a_name == NULL || op_name == NULL || b_name == NULL ) {
				printf("Im need numbers and operation,im dont love null :/");
			}else {
				double n1 = atof(a_name);
				double n2 = atof(b_name); // превращаем строчки в числа(тип double)
				switch(op) {
					case '+': res = n1 + n2; break; // сложение
					case '-': res = n1 - n2; break; // вычитание
					case '*': res = n1 * n2; break; // умножение
					case '/': if(n2 != 0) res = n1 / n2; break; // деление
			    }
			    for(int i = 0; i < count_variab; i++ ) {
			    	if(strcmp(memory[i].name,"res") == 0) {
			    		w_ind = i;
			    		break;
			    	}
			    }
			    if(w_ind != -1) {
			        sprintf(memory[w_ind].val,"%f",res); // тут мы проверяем нету ли уже созданных res
			    }else {
			    	sprintf(memory[count_variab].name,"%s","res"); // запоминаем значение для ячейки name,что бы не было погрешности с druck
			    	sprintf(memory[count_variab].val,"%f",res); // запоминаем значение для ячейки val,что бы делать корректные расчеты
			    	count_variab++;
			    }
            }
		    printf("%f\n",res); // выводим 
		}
		else if(strcmp (command, "hilfe") == 0) {
			printf("== ZITRONE PL COMMANDS ==\ndruck - print for gods(druck hello,smirnow)\nerstellen - create a popka(erstellen x 10)\nberech - math logic(berech 10 + 10 and conclusion res)\n");
		}
		else if( strcmp (command , "druck") == 0) {
            f_name = strtok(NULL," "); // проверяем значение после druck(сам druck - command,все что после command - NULL)
	        while(f_name != NULL ) { // проходимся по всему f_name   
	           found_f_name = false;
	   		    for(int i = 0; i  < count_variab; i++ ) {
					if(strcmp(memory[i].name,f_name) == 0) { // если находим выводим
				        printf("%s ",memory[i].val);
					    found_f_name = true; // ставим found_f_name в активное состояние для поиска
						break;
					}
				}
				if(!found_f_name) { // если нет выводим то что было напечатано в f_name
					printf("%s ",f_name);
				}
				f_name  = strtok(NULL, " "); // попка три попки четыре попки
			}
		}
		else {  // если вы не вводите команду вам выдает ошибку.
			printf( " Error: dont have %s \n" ,command ) ;
		}
	}
}
// команды - druck - вывод, erstellen - создать berech - рассчитать(математика)

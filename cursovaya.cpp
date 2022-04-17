#include <iostream>
#include <cstring> 
#include <ctime> 

using namespace std;


const int arr_size = 80;

struct Cargos{
	int number;
	int size;
	int time;
	int content_size;
	struct Cargos *next;
};

struct Cells{
	int number;
	int size;
	int content_size;
	int size_of_cargos_data;
	int cargos_data[arr_size][arr_size]; // {{1 , 20} , {number , size}}
	struct Cells *next;
};


// КОНТЕЙНЕРЫ
Cells* head = NULL; // Переменная для 1 элемента списка 
Cells* current; // Переменная структуры
Cells *to_put_cell;
Cells* tail = NULL; // Переменная для крайнего элемента списка

// ГРУЗЫ
Cargos* cargo_head = NULL; // Переменная для 1 элемента списка
Cargos* cargo_current; // Переменная структуры
Cargos* cargo_tail = NULL; // Переменная для крайнего элемента списка

int count_of_cells = 0; // количество отсеков
int cargo_number = 0; 

const int ratio_for_cell_size = 10;
const int ratio_for_cargo_size = 2; // rand() % 5;
const int ratio_for_cargo_time = 10;


const int hundred = 100;
const double p = 0.6;
const int started_count_of_cargos = 30;

int working_time = 0;
int start_time , end_time;

bool isAdded = false;

void cells_list(){
	cout << "Введите количество отсеков: ";

	cin >> count_of_cells;

	while(count_of_cells <= 0){
		cout << "Пожалуйста повторите попытку. Количество отсеков не может быть меньше 1." << endl;
		cin >> count_of_cells;
	}

	if (head == NULL) {
		head = new Cells;
		head -> size = ratio_for_cell_size;
		head -> number = 1;
		head -> content_size = 0;
		cout << "CELL SIZE " << head -> size << endl;
		tail = head;
	}

	for (int i = 2; i <= count_of_cells; i++) { 
		current = new Cells;
		current -> number = i;
		current -> size = i * ratio_for_cell_size;
		cout << "CELL SIZE " << current -> size << endl;
		current -> content_size = 0;
		tail -> next = current;
		tail = current;
	}
}

void onAddCargo(){
	if (cargo_head == NULL) {
		cargo_head = new Cargos;
		cargo_head -> size = ratio_for_cargo_size;
		cargo_head -> time = ratio_for_cargo_time;
		cargo_head -> number = 1;
		cargo_tail = cargo_head;
		cout<< "CARGO SIZE " << cargo_head -> size << endl;
	}else { 
		cargo_current = new Cargos;
		cargo_current -> number = cargo_tail -> number + 1;
		cargo_current -> size = (cargo_current -> number) * ratio_for_cargo_size;
		cargo_current -> time = (cargo_current -> number) * ratio_for_cargo_time;
		cout<< "CARGO SIZE " << cargo_current -> size << endl;
		cargo_tail -> next = cargo_current;
		cargo_tail = cargo_current;
	}
}


Cells* canToPushIn(int current_cell_number , int needed_size_to_put){
	to_put_cell = head;
	bool isFound = false;
	Cells *needed_cells;

	while(!isFound){
		if((current_cell_number != to_put_cell -> number) && (to_put_cell -> size - to_put_cell -> content_size >= needed_size_to_put)){
			isFound = true;
			needed_cells = to_put_cell;
		}
		to_put_cell = to_put_cell -> next;
	}

	return needed_cells;

}

// 1. Заполняем каждый отсек под потолок DONE
// 2. Если остались грузы, ищем минимальное достаточное для переноса в другой отсек, и вставляем в другой отсек IN PROGRESS

// Сделать перенос груза в другой отсек и вставку нынешнего груза в освобожденный отсек

// разобраться что вернуть в canToPushIn и обработать это

void retake_cargos(Cargos *cargo_to_change_and_put){
	current = head;

	int memory_number = 0;

	int min_free_place = 100000000;
	int free_place = 0;

	while(current != tail -> next){
		for(int i = 0 ; i < current -> size_of_cargos_data ; i++){
			free_place = current -> size - current -> content_size + current -> cargos_data[i][1];
			cout << "FREE " << free_place << " CARGO " << cargo_to_change_and_put -> size  << endl;


			if(free_place >= (cargo_to_change_and_put -> size) && free_place < min_free_place){
				// to_put_cell = canToPushIn(current -> number , cargo_to_change_and_put -> size);
				min_free_place = free_place;
				memory_number = current -> cargos_data[i][0];
				printf("Заменим груз нынешний на груз из массива №%d \n" , memory_number);

				//сделать добавление в cargos_data
			}
		}
		cout << "\n";
		current = current -> next;
	}	
}


void cargos_sort(Cargos *cargo_to_sort){
	current = head; // контейнер

	Cargos *head_for_retake = cargo_to_sort;

	int free_space;
	int counter = 0;

	while(current != tail -> next){
		free_space = current -> size - current -> content_size;
		
		if(free_space >= cargo_to_sort -> size){
			current -> content_size += cargo_to_sort -> size;
			current -> cargos_data[counter][0] = cargo_to_sort -> number;
			current -> cargos_data[counter][1] = cargo_to_sort -> size;
			current -> size_of_cargos_data += 1;
			printf("В конт #%d добавлен груз размером %d и теперь заполненость %d \n" , current->number, current-> cargos_data[counter][1] , current->content_size);
			cargo_to_sort = cargo_to_sort -> next;
			counter++;
		}else{
			counter = 0;
			current = current -> next;
		}
	}

	// while(cargo_to_sort != cargo_tail -> next){
	// 	retake_cargos(cargo_to_sort);
	// 	cargo_to_sort = cargo_to_sort -> next;
	// }
}


int main(){
	setlocale(LC_ALL, "rus");
	
	cells_list();

	for(int i = 0 ; i < started_count_of_cargos; i++){
		onAddCargo();
	}
	cargos_sort(cargo_head);


	// Cells* head = NULL; // Переменная для 1 элемента списка 
	// Cells* current; // Переменная структуры
	// Cells* tail = NULL; // Переменная для крайнего элемента списка

	// Cargos* cargo_head = NULL; // Переменная для 1 элемента списка
	// Cargos* cargo_current; // Переменная структуры
	// Cargos* cargo_tail = NULL; // Переменная для крайнего элемента списка	

	// cargos_sort();



	// while(working_time <= 500){
	// 	start_time = clock(); // в миллисекундах
	// 	srand(time(0)); // для абсолютно случайного значения числа
	// 	int random_number = rand() % hundred + 1; // диапазон от 1 до 100
	// 	while(random_number <= p * hundred){
	// 		onAddCargo();
	// 		printf("Добавлен новый груз №%d с размером %d и временем работы %d.\n", cargo_tail -> number , cargo_tail -> size , cargo_tail -> time);
	// 		random_number = rand() % hundred + 1;
	// 	}
	// 	cout << endl;

	// 	end_time = clock();

	// 	working_time += (end_time - start_time);

	// 	Cargos *head_for_deleting = cargo_head;
	// 	Cargos *delete_cargo = cargo_head;
	// 	Cargos *prev = cargo_head;

	
	// 	while(head_for_deleting != cargo_tail -> next){
	// 		if((head_for_deleting -> time) <= working_time){
	// 			printf("Удален груз №%d с временем работы %d при времени работы программы %d \n" , head_for_deleting -> number , (head_for_deleting -> time)  , working_time );

	// 			if(head_for_deleting == cargo_head){
	// 				delete_cargo = head_for_deleting;
	// 				head_for_deleting = head_for_deleting -> next; 
	// 				prev = head_for_deleting;
	// 				cargo_head = cargo_head -> next;
	// 				delete delete_cargo;

	// 			}
				
	// 			// else if(head_for_deleting == cargo_tail){
	// 			// 	delete_cargo = head_for_deleting;
	// 			// 	prev -> next = head_for_deleting -> next;  // то есть NULL
	// 			// 	delete delete_cargo;
	// 			// 	head_for_deleting = prev -> next;
	// 			// 	cargo_tail = prev;
	// 			// }else {
	// 			// 	delete_cargo = head_for_deleting;
	// 			// 	prev -> next = delete_cargo -> next;
	// 			// 	delete delete_cargo;
	// 			// 	head_for_deleting = prev -> next;	
	// 			// }

	// 		} else{

	// 			prev = head_for_deleting;
	// 			head_for_deleting = head_for_deleting -> next;
	// 		}
	// 	}
		
	// 	cout << endl;

	// }
	
	return 0;
}


// На роботизированном складе имеется n отсеков, в которые робот может размещать грузы. 
// Отсек с номером i имеет вместимость ci. В каждую единицу времени с вероятностью p на склад может поступить груз.
//  Груз с номером j имеет размер sj, хранится на складе время tj, после чего забирается со склада. 
//  Вместимость отсека и размер груза имеют одну и ту же размерность. 
//  Если в отсеке с вместимостью c находится несколько грузов с суммарным размером d, то свободное место в этом отсеке равно c – d. 
// Когда груз с номером j поступает на склад, робот сначала пытается найти отсек, в котором достаточно свободного места для размещения этого груза.
//  Если отсеков, в которых достаточно свободного места, несколько, то робот помещает груз в тот из них, в котором свободного места меньше. 
//  Если и таких отсеков несколько, то робот выбирает отсек с минимальным номером. 
// Если отсеков с достаточным количеством свободного места нет, робот пытается переместить грузы, уже расположенные в отсеках. 
// Для этого он пытается найти такой отсек и такой груз в нем, что перемещение его в другой отсек обеспечивает достаточное количество свободного места
//  для размещения поступившего груза. Если таких вариантов перемещения грузов несколько, то выбирается тот вариант, в котором потребуется перемещение 
//  груза с минимальным размером. Если и таких вариантов несколько, то выбирается вариант перемещения, при котором в отсеке, из которого перемещается груз,
//   свободное место после перемещения этого груза будет минимально, а при прочих равных условиях — тот вариант, при котором в отсеке, куда осуществляется 
//   перемещение, свободное место после этого перемещения будет также минимально. Если и после этого остается более одного варианта, то выбирается тот вариант,
//    при котором номер перемещаемого груза минимален и номер отсека, в который он перемещается, – также минимален. Если варианта с перемещением одного груза найти 
//    не удалось, то груз не принимается на склад. 
// Требуется написать программу, которая моделирует работу склада и выводит на экран состояние склада и последовательность действий, выполняемых роботом. 
// Параметры склада и груза задаются случайным образом в виде целых чисел, p<1. 
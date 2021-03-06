#include <iostream>
#include <cstring> 
#include <ctime> 

using namespace std;


const int arr_size = 100;

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

Cargos *forgotten_cargos = NULL;
Cargos *forgotten_cargos_head = NULL;
Cargos *forgotten_cargos_tail = NULL;



int count_of_cells = 0; // количество отсеков
int cargo_number = 0; 

const int ratio_for_cell_size = 10;
const int ratio_for_cargo_size = 2; // rand() % 5;
const int ratio_for_cargo_time = 10;


const int hundred = 100;
const double p = 60;
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
		cout << "Размер вашего отсека: " << head -> size << endl;
		tail = head;
	}

	for (int i = 2; i <= count_of_cells; i++) { 
		current = new Cells;
		current -> number = i;
		current -> size = i * ratio_for_cell_size;
		cout << "Размер вашего отсека: " << current -> size << endl;
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
		// cout<< "CARGO SIZE " << cargo_head -> size << endl;
	}else { 
		cargo_current = new Cargos;
		cargo_current -> number = cargo_tail -> number + 1;
		cargo_current -> size = (cargo_current -> number) * ratio_for_cargo_size;
		cargo_current -> time = (cargo_current -> number) * ratio_for_cargo_time;
		// cout<< "CARGO SIZE " << cargo_current -> size << endl;
		cargo_tail -> next = cargo_current;
		cargo_tail = cargo_current;
	}
}

bool canToPushIn(int current_cell_number , int needed_size_to_put , int cargos_number , int position_of_retaked_cargo , Cells *current){ // перенос из старого отсека в новый
	Cells* recells = head; // новый отсек
	bool isFound = false;

	while(!isFound && recells != tail -> next){	 // пока мы не нашли для груза с которым хотим поменяться свободное место в других отсеках, ищем
		if((current_cell_number != recells -> number) && (recells -> size - recells -> content_size >= needed_size_to_put)){ // нашли куда перенести груз
			isFound = true;	

			//перенос старого груза в новый отсек
			recells -> size_of_cargos_data++; 
			recells -> cargos_data[recells -> size_of_cargos_data - 1][0] = current -> cargos_data[position_of_retaked_cargo][0];
			recells -> cargos_data[recells -> size_of_cargos_data - 1][1] = current -> cargos_data[position_of_retaked_cargo][1];
			recells -> content_size += current -> cargos_data[position_of_retaked_cargo][1];
			//запись нового груза на место старого груза
			current -> cargos_data[position_of_retaked_cargo][0] = cargos_number;
			current -> cargos_data[position_of_retaked_cargo][1] = needed_size_to_put;
			current -> content_size -=  current -> cargos_data[position_of_retaked_cargo][1]; // убираем размеры старого груза
			current -> content_size += needed_size_to_put;// добавляем размеры нового груза
		}	
		recells = recells -> next;
	}
	return isFound;
}

void retake_cargos(Cargos *cargo_to_change_and_put){ // вставить cargo_to_change_and_put
	current = head;
	int min_free_place = 100000000;
	int free_place = 0;
	bool isPushed;

	int cell_memory_number = 0;
	int cargo_memory_number = 0;
	int cargo_memory_size = 0;
	int memory_which_will_retake = 0;
	Cells *memory_cell;
	bool isFoundMin = false;
	

	while(current != tail -> next){ //пока не все отсеки попробованы

		for(int i = 0 ; i < current -> size_of_cargos_data ; i++){ // ищем с каким грузом заменить данный
			free_place = current -> size - current -> content_size + current -> cargos_data[i][1];

			if(free_place >= (cargo_to_change_and_put -> size) && free_place < min_free_place){ // найден груз с которым можно заменить нынешний с минимальным местом
				min_free_place = free_place;
				cell_memory_number = current -> number;
				cargo_memory_size = cargo_to_change_and_put -> size;
				cargo_memory_number = cargo_to_change_and_put -> number;
				memory_which_will_retake = i;
				memory_cell = current;
				isFoundMin = true;
			}
		}
		
		current = current -> next;
	}	

	cout << endl;
	if(isFoundMin){
		isPushed = canToPushIn(cell_memory_number , cargo_memory_size ,cargo_memory_number , memory_which_will_retake , memory_cell);

		if(isPushed){
			printf("В отсек %d был добавлен груз №%d \n" , current -> number , cargo_to_change_and_put -> number);
		}else{
			//запомним груз который не получилось удалить сейчас, возможно после удаления некоторых грузов, место освободиться

			if(forgotten_cargos_head == NULL){
				forgotten_cargos_head = new Cargos;
				forgotten_cargos_head -> number  = cargo_to_change_and_put -> number;
				forgotten_cargos_head -> size  = cargo_to_change_and_put -> size;
				forgotten_cargos_head -> time  = cargo_to_change_and_put -> time;
				forgotten_cargos_tail = forgotten_cargos_head;
			}else{
				forgotten_cargos = new Cargos;
				forgotten_cargos -> number  = cargo_to_change_and_put -> number;
				forgotten_cargos -> size  = cargo_to_change_and_put -> size;
				forgotten_cargos -> time  = cargo_to_change_and_put -> time;
				forgotten_cargos_tail -> next = forgotten_cargos;
				forgotten_cargos_tail = forgotten_cargos;
			}
			
			printf("Груз №%d невозможно ни с кем заменить, так как нет свободного места\n" , cargo_to_change_and_put -> number);
		}
	}else{
		if(forgotten_cargos_head == NULL){
			forgotten_cargos_head = new Cargos;
			forgotten_cargos_head -> number  = cargo_to_change_and_put -> number;
			forgotten_cargos_head -> size  = cargo_to_change_and_put -> size;
			forgotten_cargos_head -> time  = cargo_to_change_and_put -> time;
			forgotten_cargos_tail = forgotten_cargos_head;
		}else{
			forgotten_cargos = new Cargos;
			forgotten_cargos -> number  = cargo_to_change_and_put -> number;
			forgotten_cargos -> size  = cargo_to_change_and_put -> size;
			forgotten_cargos -> time  = cargo_to_change_and_put -> time;
			forgotten_cargos_tail -> next = forgotten_cargos;
			forgotten_cargos_tail = forgotten_cargos;
		}
			
		printf("Груз №%d невозможно ни с кем заменить, так как нет свободного места\n" , cargo_to_change_and_put -> number);
	}

}

void cargos_sort(Cargos *cargo_to_sort , int start_point , Cargos *current_cargo_tail){
	current = head; // контейнер

	int free_space;
	int counter = 0;

	while(cargo_to_sort -> number <= start_point){
		cargo_to_sort = cargo_to_sort -> next;
	}

	while(current != tail -> next){

		free_space = current -> size - current -> content_size;
		
		if(cargo_to_sort != current_cargo_tail -> next){

			if(free_space >= cargo_to_sort -> size){
				current -> content_size += cargo_to_sort -> size;
				current -> cargos_data[counter][0] = cargo_to_sort -> number;
				current -> cargos_data[counter][1] = cargo_to_sort -> size;
				current -> size_of_cargos_data += 1;
				printf("В конт #%d добавлен груз размером %d и номером %d в массив заполненность: %d \n" , current->number, current-> cargos_data[counter][1] , current-> cargos_data[counter][0] , current -> content_size);
				cargo_to_sort = cargo_to_sort -> next;
				counter++;
			}else{
				counter = 0;
				current = current -> next;
			}

		}else{
			current = tail -> next;
		}
	}

	while(cargo_to_sort != current_cargo_tail -> next){
		retake_cargos(cargo_to_sort);
		cargo_to_sort = cargo_to_sort -> next;
	}
	cout << endl;

}

void checker(Cargos *cargo_to_delete){
	current = head;
	bool isFind = false;

	while(current != tail -> next && !isFind){

		for(int i = 0 ; i < current -> size_of_cargos_data ; i++){
			if(current -> cargos_data[i][0] == cargo_to_delete -> number){ //нашли наш груз в отсеке
				isFind = true;
				
				for(int j = i ; j <  current -> size_of_cargos_data ; ++j){ //удаление из массива структуры данного груза
					current -> cargos_data[i][0] = current -> cargos_data[i + 1][0];
					current -> cargos_data[i][1] = current -> cargos_data[i + 1][1];
				}
				current -> size_of_cargos_data--; //уменьшаем длинну массива потому что удалили один элемент
				current -> content_size = cargo_to_delete -> size;
				cout << endl;
			}
		}
		current = current -> next;
	}
}


int main(){
	setlocale(LC_ALL, "rus");
	
	cells_list();

	int count_of_cargos = 0;
	int count_of_forgeted = 0;

	while(working_time <= 500){
		start_time = clock(); // в миллисекундах

		srand(time(0)); // для абсолютно случайного значения числа
		int random_number = 70; // диапазон от 1 до 100 rand() % hundred + 1

		while(random_number >= p){
			onAddCargo();
			printf("Добавлен новый груз №%d с размером %d и временем работы %d.\n", cargo_tail -> number , cargo_tail -> size , cargo_tail -> time);
			random_number--; // = rand() % hundred + 1
		}

		cout << endl;
		Cargos *forgot = forgotten_cargos_head;
		Cargos *mem;

		if(forgot != NULL){
			cargos_sort(forgotten_cargos_head , count_of_forgeted , forgotten_cargos_tail);
			
			cout << "WHILE";
			while(forgotten_cargos_head != forgotten_cargos_tail -> next ){
				mem = forgotten_cargos_head;
				forgotten_cargos_head = forgotten_cargos_head -> next;
				delete mem;
			}
			cout << "WHILE DONE";

			forgotten_cargos_head = NULL;
			forgotten_cargos_tail = NULL;
		}

		if(cargo_tail != NULL){
			cargos_sort(cargo_head , count_of_cargos , cargo_tail);
			count_of_cargos += cargo_tail -> number - count_of_cargos;
		}
		
		end_time = clock();

		working_time += (end_time - start_time);

		Cargos *head_for_deleting = cargo_head;
		Cargos *delete_cargo = cargo_head;
		Cargos *prev = cargo_head;

	
		while(head_for_deleting != cargo_tail -> next){
			if((head_for_deleting -> time) <= working_time){
				printf("Удален груз №%d с временем работы %d при времени работы программы %d \n" , head_for_deleting -> number , (head_for_deleting -> time)  , working_time );
				

				if(head_for_deleting == cargo_head){
					checker(head_for_deleting); // проверим и удалим из отсека
					delete_cargo = head_for_deleting;
					head_for_deleting = head_for_deleting -> next; 
					prev = head_for_deleting;
					cargo_head = cargo_head -> next;
					delete delete_cargo;
				}
				
			} else{
				prev = head_for_deleting;
				head_for_deleting = head_for_deleting -> next;
			}
		}
	
		// break;

		cout << endl;

	}
	
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
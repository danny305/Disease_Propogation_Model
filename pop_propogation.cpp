#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include "human_disease.h"

using namespace std;



class Population
{
	private:
		vector<Person> pop_vect;	
		int day = 0;	
		int size;
		int init_sick;
		int init_inocc;

	public:
		Population(int pop_size = 20, int initSick = 1, int initInocc = 0)
		{	
			size = pop_size;
			init_sick = initSick;
			init_inocc = initInocc;
			int sick_counter = 0;
			int inocc_counter = 0;
			for (int x=0; x < pop_size; x++)
			{
				if (sick_counter < init_sick)
				{
					Person person_obj =  Person(x,"sick");
					pop_vect.push_back(person_obj);
					sick_counter++;
				}
				else if (inocc_counter < init_inocc)
				{	
					Person person_obj =  Person(x,"inocculated");
					pop_vect.push_back(person_obj);
					inocc_counter++;	
				}
				else
				{
					Person person_obj = Person(x, "susceptible");
					pop_vect.push_back(person_obj);
				}
			}		
		}		
		
		void update ()
		{
			for (auto &person_obj : pop_vect)
			{	
				person_obj.update();
			}
			day++;
		}
		
		void status()
		{
			cout << "The size of the Population: " << size << endl;
			cout << "The People Vector: " << endl;

			for (auto person_obj : pop_vect)
				person_obj.status_string();
		}	
		
		void update_pop_to_day(int n = 1)
		{
			for (int i=0;i < n; i++)
			{
				for (auto &person_obj : pop_vect)
                        	{
                                	person_obj.update();
                        	}
                        day++;
			}
			
		}
		
		vector <map<string,int>> getPopState ()
		{
			vector <map <string,int>> pop_state;
			for (auto &person_obj : pop_vect)
			{
				map<string, int> person_state = person_obj.getState();
				pop_state.push_back(person_state);
			}
			return pop_state;
		}

		void outputPopState () 
		{
			cout << "Day: " << day << endl << endl;
			pop_vect[0].outputStateCSVheader();
			for (auto &person_obj : pop_vect)
			{
				person_obj.outputStateCSV();
			}
		}

		string getDate () 
		{
			time_t now = time(0);
			tm * dt = localtime(&now);

			return to_string(dt->tm_mon + 1) + "-" + to_string(dt->tm_mday) + "-" + to_string(dt->tm_year + 1900) ;

		}

		string getTime ()
		{
			time_t now = time(0);
			tm * dt = localtime(&now);

			return to_string(dt->tm_hour) + ":" + to_string(dt->tm_min) ;
		}
/*
		ofstream * create_file_w_CSVheader ()
		{
			ofstream outputfile;
			outputfile.open("dis_prop_" + getDate() + "_" + getTime() + ".csv");
			outputfile << pop_vect[0].outputStateCSVheader();
			return &outputfile;
		}

		void outputPopState2File ()
		{
			//need to figure out how I am going to get header row to print only once in a module fashon. 
			for (auto &person_obj : pop_vect)
			{
				file_handle << person_obj.outputStateCSV();
			}	
		}
*/

		void pop_interact(int num_interact = 10,float inf_prob = .1, bool inc_day = false)
		{
			map <int, int> interaction_counts;
			for (auto &person_obj : pop_vect)
				interaction_counts[person_obj.getID()] = 0;

			for (auto &person_obj : pop_vect)
			{
				map <int,bool> chosen_people;
				while (interaction_counts[person_obj.getID()] < num_interact)
				{
					int random_person = (size) * 1. * rand()/RAND_MAX;
					
				//	cout << "Interaction with Person: " << random_person << endl;

					if (random_person == person_obj.getID() || chosen_people[random_person] == true)
					{
				//		cout << "This random person is not valid: " << random_person << endl;
						continue;
					}
					chosen_people[random_person] = true;
					interaction_counts[person_obj.getID()] = interaction_counts[person_obj.getID()] + 1;
					interaction_counts[random_person] = interaction_counts[random_person] +1;

					float random_num = 1. * rand()/RAND_MAX;
				//	cout << "The random number is: " << random_num << endl;

					if (random_num < inf_prob)
					{	
						if (person_obj.getHealth() == "sick" && 
						pop_vect[random_person].getHealth() == "susceptible")		
						{
							cout << "person: " << person_obj.getID() << " is infecting rand person: " <<
							pop_vect[random_person].getID() << endl;
							pop_vect[random_person].infect();
						}
						else if (pop_vect[random_person].getHealth() == "sick" && 
						person_obj.getHealth() == "susceptible")	
						{
							cout << "Rand person: " << pop_vect[random_person].getID() << 
							" is infecting person: " << person_obj.getID() << endl;
							person_obj.infect();
						}
						else
							continue;
						
						//cout << "INFECTION ALERT!" << endl;
						//person_obj.status_string();
						//pop_vect[random_person].status_string();
					}

				//	cout << "Person ID: " << person_obj.getID() << endl << "	Interaction count: " << 
				//	interaction_counts[person_obj.getID()] << endl;
				}

			} 
			if (inc_day){
				day++; 
			}			
		}
		
		void propogate (bool write_2_file=false) 
		{
			
			srand(time(NULL));
			int sick_counter = 0;
			int prop_days = 0;
			for ( auto &person_obj : pop_vect )
			{
				if (person_obj.getHealth() == "sick")
					sick_counter++;
			}
			ofstream outputfile;
			if (write_2_file)					
			{
				outputfile.open("./csv_data/dis_prop_" + getDate() + "_" + getTime() + ".csv");
				outputfile << pop_vect[0].outputStateCSVheader() << endl;
			}
			while (sick_counter != 0)
			{
				update_pop_to_day();
				pop_interact();
				if (write_2_file)
				{
					for (auto &person_obj : pop_vect)
					{
						outputfile << person_obj.outputStateCSV() << endl;
					}	
				}
				
				else
					outputPopState();
				
				cout << "----------------------------------------------------------------------------"<< endl << endl;		
				prop_days++;
				int  current_sick = 0;
						
				for ( auto &person_obj : pop_vect )
				{
					if (person_obj.getHealth() == "sick")
						current_sick++;
				}
				sick_counter = current_sick;
			}
			outputfile.close();		
				
		}

};



int main()
{
	/*
 	Person roosh = Person(1);
	roosh.status_string();
	*/
	
	Population htown(20,1,0);
	cout << htown.getDate() << endl;
	//htown.status();
 	//htown.update_pop_to_day(3);
	//htown.outputPopState();
	
	cout << "---------------------------------------------------------------------------------------------------"<< endl << endl;
	//htown.pop_interact();
	
	htown.propogate(true);
	cout << "---------------------------------------------------------------------------------------------------"<< endl << endl;
	
	//htown.outputPopState();
}

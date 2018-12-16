#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <ctime>


#include <typeinfo>
#include <exception>

#include "human_muted.h"

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
		Population(int pop_size = 20, int initSick = 1, int initInocc = 0, int sick_days = 7)
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
					Person person_obj =  Person(x,"sick", sick_days);
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
	//		cout << "Day: " << day << endl << endl;
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

		void pop_interact(int num_interact = 10,float inf_prob = .1, int sick_days = 7)
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
							//cant infect others the same day you were infected.
							if (person_obj.getSickDaysLeft() == sick_days)
								continue;
				
	//						cout << "person: " << person_obj.getID() << " is infecting rand person: " <<
	//						pop_vect[random_person].getID() << endl;
							pop_vect[random_person].infect(sick_days);
						}

						else if (pop_vect[random_person].getHealth() == "sick" && 
						person_obj.getHealth() == "susceptible")	
						{
							//Cant infect others the same day you were infected.
							if (pop_vect[random_person].getSickDaysLeft() == sick_days)
								continue;
				
	/*						cout << "Rand person: " << pop_vect[random_person].getID() << 
							" is infecting person: " << person_obj.getID() << endl;
	*/						person_obj.infect(sick_days);
						}

						else
							continue;
						
					}

				}

			} 
					
		}
		
		void propogate (int num_interact=10, float inf_prob=0.05, int sick_days=7, bool write_2_file=false, int init_inocc=0) 
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
				outputfile.open("../csv_data/dis_prop_" + getDate() + "_" + getTime() + "-" + to_string(init_inocc) + ".csv");
				outputfile << pop_vect[0].outputStateCSVheader() << endl;
			}
			while (sick_counter != 0)
			{
				update_pop_to_day();
				pop_interact(num_interact,inf_prob, sick_days);
				if (write_2_file)
				{
					for (auto &person_obj : pop_vect)
					{
						outputfile << person_obj.outputStateCSV() << endl << endl;
					}	
				}
				
				else
					outputPopState();
				
//				cout << "-------------------------------------------------------"<< endl << endl;		
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


void run_dis_propogator (int pop_size=100, int start_sick=1, int start_inocc=0, int sick_days=7, 
			 int interactions=10, float inf_prob=0.05, bool mk_file=true)
{
	Population ATX(pop_size, start_sick, start_inocc,sick_days);
	
	ATX.propogate(interactions, inf_prob, sick_days, mk_file, start_inocc);

}



int main(int argc, char* argv[])
{
	/*
 	Person roosh = Person(1);
	roosh.status_string();
	*/
	
	//Population htown(20,1,0);
	//cout << htown.getDate() << endl;
	//htown.status();
 	//htown.update_pop_to_day(3);
	//htown.outputPopState();
	
	//htown.pop_interact();
	//htown.propogate(true);
	
	if (argc != 2 && argc != 8) 
	{
	
	    cout << endl << "###########################################################################################"<< endl << endl;
     
        cout << "Please provide all command line arguments in the following order:" << endl <<
		"Population Size (int), Initial Sick (int), Initial Vaccinated (int),\nLength of sickness(days) (int), " <<
		"Interactions per day (int), Infection Probability (float) < 1.00,\noutput file(bool: true or false)" << endl << 
		"If you wish to pass default arguments, then provide the -d flag." <<endl;
        
	    cout << endl << "###########################################################################################"<< endl << endl;
    
    }

	else if ( argc == 2)
	{	//The -d flag stands for default parameters. 
		string arg_1(argv[1]);
		if (arg_1 == "-d")
		{
			run_dis_propogator();
		}
		else 
		{
			cout << "The argument you provided is invalid. Exiting.." << endl;
			exit(1);
		}
	}

	else if ( argc == 8)
	{
		try
		{
			int val_1 = stoi(argv[1]);
			int val_2 = stoi(argv[2]);
			int val_3 = stoi(argv[3]);
			int val_4 = stoi(argv[4]);
			int val_5 = stoi(argv[5]);
			float val_6 = atof(argv[6]);
			string logical(argv[7]);
			bool val_7;

			if (logical == "true")
				val_7 = true;

			else if (logical == "false")
				val_7 = false;

			else
			{
				throw"The make_file argument was neither true or false.";
			}

				
		/*	cout << "Population: " << argv[1] << " converted: " << val_1 << endl;
			cout << "Initial Sick: " << argv[2] << " converted: " << val_2 << endl;
			cout << "Initial Vaccinated: " << argv[3] << " converted: " << val_3 << endl;
			cout << "Sickness Duration in days: " << argv[4] << " converted: " << val_4 << endl;
			cout << "Interactions per day per person: " << argv[5] << " converted: " << val_5 << endl;
			cout << "Infection probability: " << argv[6] << " converted: " << val_6 << endl;
			cout << "Create File: " << argv[7] << " converted: " << val_7 << endl;
        */
			if (val_2 > val_1)
			{	
				throw "The number of initial infected is greater than the population.";
			}

			else if (val_3 > val_1)
			{	
				throw "The number of initial vaccinated is greater than the population.";
			}


			else if (val_5 > val_1)
			{
				throw "The number of interactions is greater than the population.";
			}

			else if (val_6 > 1)
			{
				throw "Infection Probability is greater than 1.";
			}

			else
				run_dis_propogator(val_1, val_2, val_3, val_4, val_5, val_6, val_7);
		}
		
		catch (const char* msg)
		{
			cout << "An exception occurred." << endl;
			cout << msg << endl;
			cout << "Exitting.." << endl;
			exit(1);
		}
	}
	

//	run_dis_propogator();

	
	//htown.outputPopState();
}


//Danny Diaz
//dd32387
//11-21-18
//Final Project: Disease Propogation Model

#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <map>

using namespace std;


class Person
{
	private:
		int age;
		int id;
		string health;
		int sick_days_left;
		vector<string> states = {"susceptible", "recovered","inocculated","sick"};
		map<string, int> state;
		map<string, int> health_conv = {{"susceptible", 0}, { "recovered", 1},
					        {"inocculated", 2}, {"sick", -1}};

	public:
		Person(int uniq_id, string dis_stat = "susceptible")
		{
			id = uniq_id;
			health = dis_stat;
			age = 0;
			if (health == "sick")
				sick_days_left = 5;
		}
		
		int getID()
		{
			return id;
		}

		string getHealth()
		{
			return health;
		}
		
		int getSickDaysLeft()
		{
			return sick_days_left;
		}
		
		void status_string()
		{
			cout << "Person: " << id << " health state is: " << health << endl;
			if (health == "sick")
				cout << "They will be sick for " << sick_days_left << 
				" more days." << endl; 
		}

		void infect(int days = 5)
		{
			if (health == "susceptible")
				{
				health = "sick";
				sick_days_left = days;
				}
		}

		void update()
		{
			age++;
			if(health == "sick" && sick_days_left != 0)
			{
				sick_days_left--;
			}
			else if(health == "sick" && sick_days_left == 0)
			{
				health = "recovered";
			}
		//	status_string();
		}
		
		bool is_stable()
		{
			if (health == "recovered")
				return true;
			else
				return false; 
		}
		map<string, int> getState () 
		{
			state["age"] = age; 
			state["id"] = id;
			state["health"] = health_conv[health];
			state["sick_days_left"] = sick_days_left; 
			
			return state;
		}		
		void outputState () 
		{
			getState();
			cout << "Person ID: " << state["id"]
			<< " age: " << state["age"] 
			<< " health: " << state["health"]
			<< " sick_days_left: " << state["sick_days_left"] << endl;
		}

		string outputStateCSVheader ()
		{	
			cout << "Person ID, " << "health, " << "sick_days_left, " << "age" << endl;
			
			return "Person ID, health, sick_days_left, age";

		}		
	
		string outputStateCSV () 
		{
			getState();
			cout << state["id"] << ",            " << state["health"] << ",         " <<  
			 state["sick_days_left"] << ",        " << state["age"] << endl;
		
			
			return to_string(state["id"]) + ",            " + to_string(state["health"]) + ",         " +  
			 to_string(state["sick_days_left"]) + ",        " + to_string(state["age"]);

		}
		/*
		void infect(int days)
		{	
			if (health != "susceptible")
			{
				return;
			}
			else if (health )
			{	
				srand(time(NULL));
				int bad_luck =  100. * rand()/RAND_MAX;
				cout << bad_luck << endl;
				if (bad_luck >90)
				{
					health = "sick";
					sick_days_left = days; 
				}
			}
		}
		*/
};

/*
int main()
{
	Person mikey = Person(1);
	Person james = Person(2);
	james.infect(5);
	mikey.infect(5);
	while (! mikey.is_stable())
	{
		mikey.update();
		cout << mikey.is_stable() << endl;
				
		james.update();
		cout << james.is_stable() << endl;
	}
}
*/

#include<iostream>
#include<cstdio>
#include<vector>
#include<fstream>
#include<string>
#include<cstdlib>
#include<cmath>
#include <ctime>
using namespace std;

string NPE;
long index=0;
float area=0.0;
float aspect_ratio=0.0;
char modname;
char filename[100];
struct NPEnode
{
	vector<float> height;
	vector<float> width;
	int npam;
};

struct NPEstack
{
	vector<NPEnode> parameter;
};

struct NPEstack store;

class CostNPE
{
	public:
		void createNPE();
		void displayNPE();
		int operands();
		void operators();
		NPEnode pop();
		float cost();
		bool CheckSkewedProperty(string);
		bool CheckBallotingProperty(string);
		void M1();
		void M2();
		void M3();
		bool isOperator(char);
		float initialTemperature();
};

int main()
{
	int n = 0, j = 0, move_choice = 0 , N=0;
	float costDiff = 0.0;
	float chipratio = 2.0;
	int nmoves = 10;
	float ratio = 0.85;
	float lambdatf = 0.005;
	float epsilon = 0.001;
	int iseed = 3;
	float random = 0;
	int uphill, MT, M, reject;
	CostNPE cp;
	cout<<"Enter name of input file for getting area and aspect ratio : "<<endl;
	cin>>filename;
	cp.createNPE();
	//NPE="85H4V7H2V1V3H9H6HaVbVdVfHcHjHiVeVlVgVkH";
	cp.displayNPE();
	float initial_cost=cp.cost();
	float asmita=initial_cost;
	cout<<"The initial cost of the given NPE is : "<<initial_cost<<endl;
	while(NPE[j]!='\0')
	{
		if (NPE[j]!= 'H' && NPE[j]!= 'V')
			n++;
		j++;
	}
	cout<<"No. of basic rectangles : "<<n<<endl;
	N=nmoves*n;
	float bestCost = initial_cost;
	string bestNPE = NPE;
	float T0 = cp.initialTemperature();
	float T = T0;
	string copyNPE="", newNPE="";
	clock_t start, stop;
	double totalTime;
	start=clock();
	//Annealing starts
	srand(iseed);
	do
	{
		MT = uphill = reject = 0;
		do
		{
			copyNPE=NPE;
			move_choice = rand() % 3;
			switch(move_choice+1)
			{
				case 1 :
					cp.M1();
					break;
				case 2 :
					cp.M2();
					break;
				case 3 :
					cp.M3();
					break;
			}
			MT++;
			float NewCost = cp.cost();
			costDiff=NewCost-initial_cost;
			cout<<"The NPE after move is : "<<NPE<<endl;
			cout<<"The new cost is : "<<NewCost<<endl;
			cout<<"Cost Difference "<<costDiff<<endl;
			random = rand() / RAND_MAX;
			if ((costDiff < 0) || random < exp ( -1 * costDiff / T))
			{
				if ( costDiff > 0)
					uphill++;
				newNPE=NPE;
				if(NewCost < bestCost)
				{
					bestNPE = newNPE;
					bestCost = NewCost;
				}
				 initial_cost = NewCost;
			}
			else
			{
				NPE=copyNPE;
				reject++;
			}
//			cout <<"Reject "<< reject << endl;
//			initial_cost = NewCost;
			stop=clock();
	                totalTime = (stop - start)/(double)CLOCKS_PER_SEC;
			
		}while((uphill < N) && (MT < (2*N)) && totalTime<=30);
		//cout << "outside...\n";
		T = T < (lambdatf * T0) ? 0.1 * T : ratio * T;
		cout<<"Total Time "<<totalTime<<endl;
	}while (((reject / MT) <= 0.95) && T > epsilon && totalTime<=30);
    	cout<<"The best floorplan NPE is : " <<bestNPE<<endl;
	cout<<"\nThe Initial Cost was : "<<asmita<<endl;
	cout<<"\nThe minimum cost is "<< bestCost<<endl;
	return 0;
}

//Creates a new NPE
void CostNPE::createNPE()
{
	int j=0;
	ifstream infile;
	infile.open(filename, ios::in);
	infile>>modname>>area>>aspect_ratio;
	NPE=NPE+modname;
	while(!infile.eof() && infile>>modname>>area>>aspect_ratio)
	{
		NPE=NPE+modname;
		NPE=NPE+'V';
	}
	infile.close();
}

//Prints the corresponding NPE
void CostNPE::displayNPE()
{
	cout<<"The NPE is : "<<endl;
	//cout<<NPE.length()<<endl;
	for(int j=0;j<NPE.length();j++)
		cout<<NPE[j];
	cout<<endl;
}

//Function to calculate height and width of leaf nodes, i.e., operands in a Polish expression
int CostNPE::operands()
{
	ifstream infile;
	infile.open(filename, ios::in);
	NPEnode node;
	while (!infile.eof() && infile>>modname>>area>>aspect_ratio)
	{
		if (modname==NPE[index])
		{
			node.npam=2;
			if(node.height.size()<2)
			   node.height.resize(2);
			if (node.width.size() < 2)
                node.width.resize(2);
            if (aspect_ratio == 0)
				cout<<"Aspect Ratio violation: "<<aspect_ratio;
			//there are two values of height and width and hence we define an array of two elements for each module
	    	node.height[0] = sqrt(area / aspect_ratio); 
            node.height[1] = sqrt(aspect_ratio * area);
            node.width[0] = sqrt(aspect_ratio * area);
            node.width[1] = sqrt(area / aspect_ratio);
            if (NPE[index] != 'H' && NPE[index] != 'V')
            {
            	//if it is an operand it is pushed into the stack along with the height and width pair
                index++;
                store.parameter.push_back(node);
				infile.close();
                return 1;
            }
            else
            {
            	//if operator encountered we return 1 to the cost function
                index++;
                infile.close();
                return 0; 
            }
        }
    }
    index++;
    return 0;
}

//Function to calculate height and width of H/V nodes, i.e., operands in a Polish expression
void CostNPE::operators()
{
	NPEnode rnode, lnode, parentnode;
	rnode=pop();
	lnode=pop();
	parentnode.npam=(rnode.npam)*(lnode.npam);
	if (parentnode.height.size() != parentnode.npam)
	{
		parentnode.height.resize(parentnode.npam);
    	parentnode.width.resize(parentnode.npam);
	}
	int p=0;//variable to access parentnodes
	for (int j = 0; j<rnode.npam; j++)
    {
        for (int k = 0; k<lnode.npam; k++)
        {
        	//i-th and j-th loops for leaf nodes
            if (NPE[index - 1] == 'V')
            {
            	//For vertical cut, that node will have the max height of its children and the sum of widths
                parentnode.height[p] = (rnode.height[j] > lnode.height[k] ? rnode.height[j] : lnode.height[k]);
                parentnode.width[p] = rnode.width[j] + lnode.width[k];
            }
            else if (NPE[index - 1] == 'H')
            {
            	//For horizontal cut the node will have the max of widths of its children and the sum of heights
                parentnode.width[p] = (rnode.width[j]>lnode.width[k] ? rnode.width[j] : lnode.width[k]);
                parentnode.height[p] = rnode.height[j] + lnode.height[k];
            }
            p++;
        }

    }
    store.parameter.push_back(parentnode); //Pushes height and width of H/V nodes into stack
}

//Pops a node from the stack
NPEnode CostNPE::pop()
{
	NPEnode out;
	out = store.parameter.back();
	store.parameter.pop_back();
	return out;
}

//Calculates cost of a floorplan/ NPE
float CostNPE::cost()
{
	int check,j;
	index=0;
	while (NPE[index] != '\0')
    {
        check = operands();
        if (check == 0)//When operands are not found operator store is called.
        {
            operators();
        }
    }
    float comparecost;
    float mincost;
    NPEnode rootnode;
    rootnode = pop();//Rootnode is popped
    mincost = rootnode.height[0] * rootnode.width[0];//Product of height[0] and width[0] to minimum cost.
    for (j = 0; j<rootnode.npam; j++)
    {
        comparecost = rootnode.height[j] * rootnode.width[j];
        if (comparecost<mincost)
            mincost = comparecost; //Compare the costs and select the minimum cost.
        
    }
    return mincost;
}

bool CostNPE::CheckSkewedProperty(string temp)
{
	for (int i = 1; i < 100; ++i)
	{
		if(temp[i-1] == temp[i])
			return false; 
	}

	return true;
}

bool CostNPE::CheckBallotingProperty(string temp)
{
	int operand_count = 0;
	int operator_count = 0;

	for (int l = 0; l<100; l++)
    	{
        	if (temp[l] != 'H'&&temp[l] != 'V')
        	{
            		operand_count++;
        	}
        	if (temp[l] == 'H' || temp[l] == 'V')
        	{
            		operator_count++;
        	}
        	if (operand_count<operator_count)
        	{
            		return false;
        	}
    	}

	return true;

}

void CostNPE::M1()
{
	int i=0 , j=0, a[100];
	char temp;
	while(i<NPE.length())
	{
		if (NPE[i] != 'H' && NPE[i] != 'V')
		{
			a[j] = i;
			j++;
		}
		i++;
	}	
//	cout<<"exit";
	int random = rand() % (j-1);
	temp = NPE [a[random]];
	NPE[a[random]] = NPE[a[random+1]];
	NPE[a[random+1]]=temp;
}

void CostNPE::M2()
{
	vector <int> operators;
	int l=0;
	for(int k=0 ; (k+1) < NPE.length(); k++)
	{
		if((NPE[k] != 'H' && NPE[k] != 'V') && (NPE[k+1] == 'H'|| NPE[k+1] == 'V'))
    	{
    		operators.push_back(k+1);
			l++;
    	}
	}
	int random = rand() % l;
	int i=operators[random];
	while(i < NPE.length())
	{
		if(NPE[i] == 'H' || NPE[i] == 'V')
    	{
    		if(NPE[i] == 'H')
    		{
    			NPE[i] = 'V';
    		}
    		else
    		{
    			NPE[i] = 'H';
    		}
    		i++;
    	}
    	else
    		break;
	}
	
}

void CostNPE::M3()
{
	CostNPE cp;
	vector< std::pair<int, int> > M3NPE;
	string NewNPE = NPE;
	for (int j = 1; j < NPE.length(); ++j)//storing the possible MOVE M3 pairs
	{
		if (cp.isOperator (NewNPE[j-1]) && !cp.isOperator (NewNPE[j]) )
		{
			M3NPE.push_back(std::pair<int,int>(j-1,j));	
		}
		else if (!cp.isOperator(NewNPE[j-1]) && cp.isOperator(NewNPE[j]))
			M3NPE.push_back(std::pair<int,int>(j-1,j));
	}
	if (M3NPE.size() == 0)
		return;
	int random = rand() % M3NPE.size();
	std::pair<int,int> indices = M3NPE[random];
	char temp = NewNPE[indices.first];//swapping the values
	//char c2 = NewNPE[indices.second];
	NewNPE[indices.first] = NewNPE[indices.second];
	NewNPE[indices.second] = temp;
	if (cp.CheckBallotingProperty(NewNPE) && cp.CheckSkewedProperty(NewNPE))
	{
		NPE = NewNPE;
	}
//	cp.displayNPE();
}

bool CostNPE::isOperator(char c)//to check if it is an operator
{
	if (c == 'H' || c == 'V')
		return true;
	return false;
}

float CostNPE::initialTemperature()
{
	int choice=0, nmoves=0;
	float T0=-1;
	float P=0.99;
	CostNPE cp;
	float oldCost = cp.cost();
	float costChange=0.0, newCost, costDiff;
	srand(time(NULL));
	while(nmoves<40)
	{
		choice = rand() % 3;
	//	cout<<"Choice "<<choice<<endl;
		switch(choice)
		{
			case 0:
				cp.M1();
				break;
			case 1:
				cp.M2();
				break;
			case 2:
				cp.M3();
				break;
		}
		newCost = cp.cost();
		costDiff = newCost - oldCost;
		//cout<<costDiff<<" new cost "<<newCost<<endl;
		oldCost = newCost;
		costChange += abs(costDiff);
		nmoves++;
	}
	float avgCostDiff = costChange / nmoves;
	if ( P < 1)
		T0*= avgCostDiff / log(P);
	else
		T0=avgCostDiff / log(P);
	return T0;
}

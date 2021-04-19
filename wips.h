#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>    // sort and erase
#include <iomanip>
#include <limits>

using namespace std;

// Storing Max x and y values for map from file
struct MapSize
{
	int xInput;
	int yInput;
}MapSize;

// Storing details for each grid area within
// 2D array struct
struct GridArea
{
	int x, y;
	bool isOccupied;
	int cityID;
	string cityName;

	int cloud, cloudIndex, pressure, pressureIndex;

	char cloudcoverLMH, pressureLMH;
};
GridArea** ga;

// Function prototypes
void pressEnter();
GridArea processFile();
vector <string> tokenizeString(string, string);
GridArea readFile(string);
GridArea readFileForPressure(string);
char getChar(int);
void displayMap(int, int);
void displayCloud(int, int);
void displayCloudLMH(int, int);
void displayPressure(int, int);
void displayPressureLMH(int, int);
void weatherReport(int, int);
int printCityName(int, int, vector <int>, int);
double getRainChance(char, char);
void printRainGraphic(double);

int main()
{
	// Main menu to run constantly until user enters 8
	string choice = "0";

	while (choice != "8")
	{
		cout << "Student ID         : 6355018" << endl;
		cout << "Student Name       : Brian Ong Boon Choon" << endl;
		cout << "------------------------------------------" << endl;
		cout << "Welcome to Weather Information Processing System!" << endl;

		cout << endl;

		cout << "1)\tRead in and process a configuration file" << endl;
		cout << "2)\tDisplay city map" << endl;
		cout << "3)\tDisplay cloud coverage map (cloudiness index)" << endl;
		cout << "4)\tDisplay cloud coverage map (LMH symbols)" << endl;
		cout << "5)\tDisplay atmospheric pressure map (pressure index)" << endl;
		cout << "6)\tDisplay atmospheric pressure map (LMH symbols)" << endl;
		cout << "7)\tShow forecast summary report" << endl;
		cout << "8)\tQuit" << endl;

		cout << endl;

		cout << "Please enter your choice: ";
		cin >> choice;

		cout << endl;

		if (choice == "1")
			// Process all files
			** ga = processFile();
		else if (choice == "2")
		{
			// Display city map
			displayMap(MapSize.xInput, MapSize.yInput);
		}
		else if (choice == "3")
		{
			// Display cloud index
			displayCloud(MapSize.xInput, MapSize.yInput);
		}
		else if (choice == "4")
		{
			// Display cloud character
			displayCloudLMH(MapSize.xInput, MapSize.yInput);
		}
		else if (choice == "5")
		{
			// Display pressure index
			displayPressure(MapSize.xInput, MapSize.yInput);
		}
		else if (choice == "6")
		{
			// Display pressure character
			displayPressureLMH(MapSize.xInput, MapSize.yInput);
		}
		else if (choice == "7")
		{
			// Weather report summary
			weatherReport(MapSize.xInput, MapSize.yInput);
		}
		else if (choice == "8")
			cout << "Thank you for using this program!" << endl;
		else
			cout << "Please enter valid input" << endl << endl;
	}

	// Deallocate memory for 2D array
	for (int i = 0; i < MapSize.xInput + 1; i++)
	{
		delete[] ga[i];
	}
	delete[] ga;

	return (0);
}

// Allow user to look at functions display before
// moving on by pressing Enter
void pressEnter()
{
	cout << "Press Enter to Continue: ";
	cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
	cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
}

// Begin reading of files
GridArea processFile()
{
	string fileName;

	// Asking user for input of file name
	cout << "[ Read in and process a configuration file ]" << endl;
	cout << "Please enter config filename: ";
	cin >> fileName;

	cout << endl;

	// Processing file
	ifstream inData;

	fstream inputFile(fileName.c_str(), fstream::in);

	string aLine;

	while (getline(inputFile, aLine))
	{
		// Getting range of x if "GridX_IdxRange" is found within line
		if (aLine.find("GridX_IdxRange") != string::npos)
		{
			// Obtain range of numbers into string
			string numberRangeStr = aLine.erase(0, 15);		// Remove "GridX_IdxRange" from aLine

			// Obtain individual numbers into vector string
			vector <string> numberIndividual = tokenizeString(numberRangeStr, "-");

			// Initialize to struct, x variables
			MapSize.xInput = atoi(numberIndividual.at(1).c_str());

			cout << "Reading in " << aLine << " ... done!" << endl;
			numberIndividual.clear();
		}

		// Getting range of y if "GridY_IdxRange" is found within line
		if (aLine.find("GridY_IdxRange") != string::npos)
		{
			// Obtain range of numbers into string
			string numberRangeStr = aLine.erase(0, 15);		// Remove "GridY_IdxRange" from aLine

			// Obtain individual numbers into vector string
			vector <string> numberIndividual = tokenizeString(numberRangeStr, "-");

			// Initialize to struct, y variables
			MapSize.yInput = atoi(numberIndividual.at(1).c_str());

			cout << "Reading in " << aLine << " ... done!" << endl;
			numberIndividual.clear();
		}
		if (MapSize.xInput != 0 && MapSize.yInput != 0)
			break;
	}

	cout << endl;

	// Allocate array memory in heap
	ga = new GridArea * [MapSize.xInput + 1];
	for (int i = 0; i < MapSize.yInput + 1; i++)
		ga[i] = new GridArea[MapSize.yInput + 1];

	// Putting x and y values into  array
	for (int xArray = 0; xArray < MapSize.xInput + 1; xArray++)
	{
		for (int yArray = 0; yArray < MapSize.yInput + 1; yArray++)
		{
			ga[xArray][yArray].x = xArray;
			ga[xArray][yArray].y = yArray;
		}
	}

	// To decide which file method to use
	int i = 0;

	cout << "Storing data from input file :" << endl;

	// Processing file
	while (getline(inputFile, aLine))
	{
		size_t pos = aLine.find(".txt");

		if (pos != string::npos)
		{
			if (i == 0)
			{
				// Get city locations into array
				**ga = readFile(aLine);
				cout << aLine << " ... done!" << endl;
			}
			else if (i == 1)
			{
				// Get cloud details into array
				**ga = readFile(aLine);
				cout << aLine << " ... done!" << endl;
			}
			else if (i == 2)
			{
				// Get pressure details into array
				**ga = readFileForPressure(aLine);
				cout << aLine << " ... done!" << endl;
			}
			i++;
		}
	}
	cout << endl;

	cout << "All records successfully stored. Going back to main menu ..." << endl;

	cout << endl;

	return **ga;
}

// Seperate strings into two based on delimiter
vector <string> tokenizeString(string input, string delimiter)
{
	size_t pos = 0;
	string token;
	vector <string> result;

	while ((pos = input.find(delimiter)) != string::npos)
	{
		token = input.substr(0, pos);
		result.push_back(token);					// Add element at the end
		input.erase(0, pos + delimiter.length());
	}
	result.push_back(input);

	return (result);
}

// Process file method for reading city and cloud files
GridArea readFile(string fileName)
{
	fstream inputFile(fileName.c_str(), fstream::in);

	string aLine;
	string nextLine;

	while (getline(inputFile, aLine))
	{
		/****************************************************************
		 * STOP READING FILE IF THERE IS NOTHING LEFT AND PREVENT ERROR *
		 ****************************************************************/
		if (aLine == "")
			break;

		aLine.erase(0, 1);			// remove first bracket

		// Obtaining value of grid x
		vector <string> firstValue = tokenizeString(aLine, ", ");

		int gridX = atoi(firstValue.at(0).c_str());



		// Moving on to next part of line
		nextLine = firstValue[1];

		// Obtaining value of grid y
		vector <string> secondValue = tokenizeString(nextLine, "]");

		int gridY = atoi(secondValue.at(0).c_str());



		// Moving on to next part of line
		nextLine = secondValue[1];
		nextLine = nextLine.erase(0, 1);		// remove first minus

		if (nextLine.size() == 2)
		{
			// Obtaining value cloud
			int cloud = stoi(nextLine);

			// Obtaining value cloudIndex
			nextLine = nextLine.substr(0, 1);		// Getting the first digit out of the two
			int cloudIndex = stoi(nextLine);

			// Obtaining cloudcover character
			char cloudcoverLMH = getChar(cloud);

			// Inserting values into array
			ga[gridX][gridY].cloud = cloud;
			ga[gridX][gridY].cloudIndex = cloudIndex;
			ga[gridX][gridY].cloudcoverLMH = cloudcoverLMH;
		}
		else if (nextLine.size() == 1)
		{
			// Obtaining value cloud
			int cloud = stoi(nextLine);

			// Obtaining value cloudIndex
			int cloudIndex = 0;

			// Obtaining cloudcover character
			char cloudcoverLMH = getChar(cloud);

			// Inserting values into array
			ga[gridX][gridY].cloud = cloud;
			ga[gridX][gridY].cloudIndex = cloudIndex;
			ga[gridX][gridY].cloudcoverLMH = cloudcoverLMH;
		}
		else
		{
			vector <string> thirdValue = tokenizeString(nextLine, "-");

			// Obtaining value of ID
			int cityID = atoi(thirdValue.at(0).c_str());

			// Moving on to next part of line
			nextLine = thirdValue[1];



			// Obtaining city name
			string cityName = nextLine;

			// Inserting values into array
			ga[gridX][gridY].cityID = cityID;
			ga[gridX][gridY].isOccupied = true;
			ga[gridX][gridY].cityName = cityName;
		}
	}
	return **ga;
}

// Process file method for pressure file
GridArea readFileForPressure(string fileName)
{
	fstream inputFile(fileName.c_str(), fstream::in);

	string aLine;
	string nextLine;

	while (getline(inputFile, aLine))
	{
		/****************************************************************
		 * STOP READING FILE IF THERE IS NOTHING LEFT AND PREVENT ERROR *
		 ****************************************************************/
		if (aLine == "")
			break;

		aLine.erase(0, 1);			// remove first bracket

		// Obtaining value of grid x
		vector <string> firstValue = tokenizeString(aLine, ", ");

		int gridX = atoi(firstValue.at(0).c_str());



		// Moving on to next part of line
		nextLine = firstValue[1];

		// Obtaining value of grid y
		vector <string> secondValue = tokenizeString(nextLine, "]");

		int gridY = atoi(secondValue.at(0).c_str());



		// Moving on to next part of line
		nextLine = secondValue[1];
		nextLine = nextLine.erase(0, 1);		// remove first minus

		if (nextLine.size() == 2)
		{
			// Obtaining value cloud
			int pressure = stoi(nextLine);

			// Obtaining value cloudIndex
			nextLine = nextLine.substr(0, 1);		// Getting the first digit out of the two
			int pressureIndex = stoi(nextLine);

			// Obtaining cloudcover character
			char pressureLMH = getChar(pressure);

			// Inserting values into array
			ga[gridX][gridY].pressure = pressure;
			ga[gridX][gridY].pressureIndex = pressureIndex;
			ga[gridX][gridY].pressureLMH = pressureLMH;
		}
		else
		{
			// Obtaining value cloud
			int pressure = stoi(nextLine);

			// Obtaining value cloudIndex
			int pressureIndex = 0;

			// Obtaining cloudcover character
			char pressureLMH = getChar(pressure);

			// Inserting values into array
			ga[gridX][gridY].pressure = pressure;
			ga[gridX][gridY].pressureIndex = pressureIndex;
			ga[gridX][gridY].pressureLMH = pressureLMH;
		}
	}
	return **ga;
}

// Get a character based on number in parameter
char getChar(int CloudorPressure)
{
	if (CloudorPressure <= 35)
		return ('L');
	else if (CloudorPressure >= 35 &&
		CloudorPressure <= 65)
		return ('M');
	else
		return ('H');
}

// Constructing the map for city
void displayMap(int xInput, int yInput)
{
	for (int y = yInput + 1; y >= -2; y--)
	{
		for (int x = -2; x <= xInput + 1; x++)
		{
			if ((y == yInput + 1 && x >= -1) ||	// Print first row of #
				(y == -1 && x >= -1) ||			// Print last row of #
				(x == -1 && y >= -1) ||			// Print first column of #
				(x == xInput + 1 && y >= -1))	// Print last column of #
				cout << "# ";
			else if (x == -2 && y >= 0 && y <= yInput) // Print y numbers
				cout << y << " ";
			else if (y == -2 && x >= 0 && x <= xInput) // Print x numbers
				cout << x << " ";
			else if (x <= xInput &&
				x >= 0 &&
				y <= yInput &&
				y >= 0 &&
				ga[x][y].isOccupied == true)
				cout << ga[x][y].cityID << " ";
			else
				cout << "  ";
		}
		cout << endl;
	}
	cout << endl;

	pressEnter();

	cout << endl;
}

// Constructing the map for cloud
void displayCloud(int xInput, int yInput)
{
	for (int y = yInput + 1; y >= -2; y--)
	{
		for (int x = -2; x <= xInput + 1; x++)
		{
			if ((y == yInput + 1 && x >= -1) ||	// Print first row of #
				(y == -1 && x >= -1) ||		// Print last row of #
				(x == -1 && y >= -1) ||		// Print first column of #
				(x == xInput + 1 && y >= -1))		// Print last column of #
				cout << "# ";
			else if (x == -2 && y >= 0 && y <= yInput) // Print y numbers
				cout << y << " ";
			else if (y == -2 && x >= 0 && x <= xInput) // Print x numbers
				cout << x << " ";
			else if (x <= xInput &&
				x >= 0 &&
				y <= yInput &&
				y >= 0)
				cout << ga[x][y].cloudIndex << " ";
			else
				cout << "  ";
		}
		cout << endl;
	}
	cout << endl;

	pressEnter();

	cout << endl;
}

// Constructing the map for cloud characters
void displayCloudLMH(int xInput, int yInput)
{
	for (int y = yInput + 1; y >= -2; y--)
	{
		for (int x = -2; x <= xInput + 1; x++)
		{
			if ((y == yInput + 1 && x >= -1) ||	// Print first row of #
				(y == -1 && x >= -1) ||		// Print last row of #
				(x == -1 && y >= -1) ||		// Print first column of #
				(x == xInput + 1 && y >= -1))		// Print last column of #
				cout << "# ";
			else if (x == -2 && y >= 0 && y <= yInput) // Print y numbers
				cout << y << " ";
			else if (y == -2 && x >= 0 && x <= xInput) // Print x numbers
				cout << x << " ";
			else if (x <= xInput &&
				x >= 0 &&
				y <= yInput &&
				y >= 0)
				cout << ga[x][y].cloudcoverLMH << " ";
			else
				cout << "  ";
		}
		cout << endl;
	}
	cout << endl;

	pressEnter();

	cout << endl;
}

// Constructing the map for pressure
void displayPressure(int xInput, int yInput)
{
	for (int y = yInput + 1; y >= -2; y--)
	{
		for (int x = -2; x <= xInput + 1; x++)
		{
			if ((y == yInput + 1 && x >= -1) ||	// Print first row of #
				(y == -1 && x >= -1) ||		// Print last row of #
				(x == -1 && y >= -1) ||		// Print first column of #
				(x == xInput + 1 && y >= -1))		// Print last column of #
				cout << "# ";
			else if (x == -2 && y >= 0 && y <= yInput) // Print y numbers
				cout << y << " ";
			else if (y == -2 && x >= 0 && x <= xInput) // Print x numbers
				cout << x << " ";
			else if (x <= xInput &&
				x >= 0 &&
				y <= yInput &&
				y >= 0)
				cout << ga[x][y].pressureIndex << " ";
			else
				cout << "  ";
		}
		cout << endl;
	}
	cout << endl;

	pressEnter();

	cout << endl;
}

// Constructing the map for pressure character
void displayPressureLMH(int xInput, int yInput)
{
	for (int y = yInput + 1; y >= -2; y--)
	{
		for (int x = -2; x <= xInput + 1; x++)
		{
			if ((y == yInput + 1 && x >= -1) ||	// Print first row of #
				(y == -1 && x >= -1) ||			// Print last row of #
				(x == -1 && y >= -1) ||			// Print first column of #
				(x == xInput + 1 && y >= -1))	// Print last column of #
				cout << "# ";
			else if (x == -2 && y >= 0 && y <= yInput) // Print y numbers
				cout << y << " ";
			else if (y == -2 && x >= 0 && x <= xInput) // Print x numbers
				cout << x << " ";
			else if (x <= xInput &&
				x >= 0 &&
				y <= yInput &&
				y >= 0)
				cout << ga[x][y].pressureLMH << " ";
			else
				cout << "  ";
		}
		cout << endl;
	}
	cout << endl;

	pressEnter();

	cout << endl;
}

// Creating weather report
void weatherReport(int xInput, int yInput)
{
	// Print title
	cout << "Weather Forecast Summary Report" << endl;
	cout << "-------------------------------" << endl;

	// Obtaining unique city IDs into int vector
	vector <int> IDs;

	for (int x = 0; x < xInput + 1; x++)
	{
		for (int y = 0; y < yInput + 1; y++)
		{
			if (ga[x][y].isOccupied == true)
				IDs.push_back({ ga[x][y].cityID });
		}
	}

	sort(IDs.begin(), IDs.end());
	IDs.erase(unique(IDs.begin(), IDs.end()), IDs.end());

	// Getting details each city based on ID
	for (unsigned int i = 0; i < IDs.size(); i++)
	{
		double cloudArea = 0;
		double cloudTotal = 0;
		double cloudAverage = 0;

		double pressureArea = 0;
		double pressureTotal = 0;
		double pressureAverage = 0;

		int runFunction = printCityName(xInput, yInput, IDs, i);

		for (int xArray = 0; xArray < xInput + 1; xArray++)
		{
			for (int yArray = 0; yArray < yInput + 1; yArray++)
			{
				if (ga[xArray][yArray].cityID == IDs[i])
				{
					cloudArea++;
					cloudTotal += ga[xArray][yArray].cloud;

					pressureArea++;
					pressureTotal += ga[xArray][yArray].pressure;
				}

				// Attempting to take values around city
				// If city is top of grid
				else if (yArray + 1 <= yInput && ga[xArray][yArray + 1].cityID == IDs[i])
				{
					cloudArea++;
					cloudTotal += ga[xArray][yArray].cloud;

					pressureArea++;
					pressureTotal += ga[xArray][yArray].pressure;
				}
				// If city is top left of grid
				else if (yArray + 1 <= yInput && xArray - 1 <= xInput && xArray - 1 >= 0 && ga[xArray- 1][yArray + 1].cityID == IDs[i])
				{
					cloudArea++;
					cloudTotal += ga[xArray][yArray].cloud;

					pressureArea++;
					pressureTotal += ga[xArray][yArray].pressure;
				}
				// If city is top right of grid
				else if (yArray + 1 <= yInput && xArray + 1 <= xInput && ga[xArray + 1][yArray + 1].cityID == IDs[i])
				{
					cloudArea++;
					cloudTotal += ga[xArray][yArray].cloud;

					pressureArea++;
					pressureTotal += ga[xArray][yArray].pressure;
				}
				// If city is left of grid
				else if (xArray - 1 <= xInput && xArray - 1 >= 0 && ga[xArray - 1][yArray].cityID == IDs[i])
				{
					cloudArea++;
					cloudTotal += ga[xArray][yArray].cloud;

					pressureArea++;
					pressureTotal += ga[xArray][yArray].pressure;
				}
				// If city is right of grid
				else if (xArray + 1 <= xInput && ga[xArray + 1][yArray].cityID == IDs[i])
				{
					cloudArea++;
					cloudTotal += ga[xArray][yArray].cloud;

					pressureArea++;
					pressureTotal += ga[xArray][yArray].pressure;
				}
				// If city is bottom of grid
				else if (yArray - 1 <= yInput && yArray - 1 >= 0 && ga[xArray][yArray - 1].cityID == IDs[i])
				{
					cloudArea++;
					cloudTotal += ga[xArray][yArray].cloud;

					pressureArea++;
					pressureTotal += ga[xArray][yArray].pressure;
				}
				// If city is bottom left of grid
				else if (yArray - 1 <= yInput && yArray - 1 >= 0 && xArray - 1 <= xInput && xArray -1 >= 0 && ga[xArray - 1][yArray - 1].cityID == IDs[i])
				{
					cloudArea++;
					cloudTotal += ga[xArray][yArray].cloud;

					pressureArea++;
					pressureTotal += ga[xArray][yArray].pressure;
				}
				// If city is bottom right of grid
				else if (yArray - 1 <= yInput && yArray - 1 >= 0 && xArray + 1 <= xInput && ga[xArray + 1][yArray - 1].cityID == IDs[i])
				{
					cloudArea++;
					cloudTotal += ga[xArray][yArray].cloud;

					pressureArea++;
					pressureTotal += ga[xArray][yArray].pressure;
				}
			}
		}
		// Calculate average of cloud and pressure
		cloudAverage = cloudTotal / cloudArea;
		pressureAverage = pressureTotal / pressureArea;

		cout << "City ID   : " << IDs[i] << endl;
		cout << "Ave. Cloud  Cover (ACC)   : " << fixed << setprecision(2) << cloudAverage
			<< " (" << getChar((int)cloudAverage) << ")" << endl;
		cout << "Ave. Pressure     (AP)    : " << fixed << setprecision(2) << pressureAverage
			<< " (" << getChar((int)pressureAverage) << ")" << endl;

		// Get rain probability based on characters from average
		double rainChance = getRainChance(getChar((int)cloudAverage), getChar((int)pressureAverage));

		cout << "Probability of Rain (%)   : " << fixed << setprecision(2) << rainChance << endl;

		// Print rain graphic
		printRainGraphic(rainChance);

		cout << endl;
	}
	pressEnter();

	cout << endl;
}

// Print unique city name once during report
int printCityName(int xInput, int yInput, vector <int> IDs, int i)
{
	for (int xArray = 0; xArray < xInput + 1; xArray++)
	{
		for (int yArray = 0; yArray < yInput + 1; yArray++)
		{
			if (ga[xArray][yArray].cityID == IDs[i])
			{
				cout << "City Name : " << ga[xArray][yArray].cityName << endl;

				return (0);
			}
		}
	}
	return (0);
}

// Get double based on characters in parameters
double getRainChance(char cloudAverage, char pressureAverage)
{
	if (cloudAverage == 'H' && pressureAverage == 'L')
		return (90);
	else if (cloudAverage == 'M' && pressureAverage == 'L')
		return (80);
	else if (cloudAverage == 'L' && pressureAverage == 'L')
		return (70);
	else if (cloudAverage == 'H' && pressureAverage == 'M')
		return (60);
	else if (cloudAverage == 'M' && pressureAverage == 'M')
		return (50);
	else if (cloudAverage == 'L' && pressureAverage == 'M')
		return (40);
	else if (cloudAverage == 'H' && pressureAverage == 'H')
		return (30);
	else if (cloudAverage == 'M' && pressureAverage == 'H')
		return (20);
	else
		return (10);
}

// Print graphic on rain based on double 
void printRainGraphic(double rainChance)
{
	if (rainChance == 90)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl;
		cout << "\\\\\\\\\\" << endl;
	}
	else if (rainChance == 80)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl;
		cout << " \\\\\\\\" << endl;
	}
	else if (rainChance == 70)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl;
		cout << "  \\\\\\" << endl;
	}
	else if (rainChance == 60)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl;
		cout << "   \\\\" << endl;
	}
	else if (rainChance == 50)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl;
		cout << "    \\" << endl;
	}
	else if (rainChance == 40)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl;
	}
	else if (rainChance == 30)
	{
		cout << "~~~" << endl;
		cout << "~~~~" << endl;
	}
	else if (rainChance == 20)
	{
		cout << "~~" << endl;
		cout << "~~~" << endl;
	}
	else
	{
		cout << "~" << endl;
		cout << "~~" << endl;
	}
}
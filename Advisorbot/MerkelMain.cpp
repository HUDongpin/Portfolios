#include "MerkelMain.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "OrderBook.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <math.h> 
#include <algorithm>
#include <ctype.h>
#include <stdio.h>

MerkelMain::MerkelMain()
{

}

/**High-level strcture, initial function of MerkelMain*/
void MerkelMain::init()
{
    std::string input;
    currentTime = orderBook.getEarliestTime();

    // print the menu of UI, get input from getUserOption() and use the input as the parameter in the function processUserOption
    while(true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}


/**print advisorbot menu for Human-Computer Interaction*/
void MerkelMain::printMenu()
{
    // C1 list all available commands
    std::cout << "C1: help " << std::endl;

    // C2 output help for the specified command
    std::cout << "C2: help cmd" << std::endl;

    // C3 list available products
    std::cout << "C3: prod " << std::endl;

    // C4 find minimum bid or ask for product in current time step
    std::cout << "C4: min " << std::endl;

    // C5 find maximum bid or ask for product in current time step
    std::cout << "C5: max " << std::endl;

    // C6 compute average ask or bid for the sent product over the sent number of timesteps
    std::cout << "C6: avg " << std::endl;

    // C7 predict max or min ask or bid for the sent product for the next timestep
    std::cout << "C7: predict " << std::endl;

    // C8 state current time in dataset, i.e. which timeframe are we looking at  
    std::cout << "C8: time " << std::endl;

    // C9 move to next time step
    std::cout << "C9: step " << std::endl;

    // C10 find median bid or ask for product in current time step  
    std::cout << "C10: median " << std::endl;

    // a long delimiter
    std::cout << "============== " << std::endl;
}


// C1: help
/**list all available commands*/
void MerkelMain::printHelp()
{
    std::cout << "Help -  The available commands are help, help <cmd>, min, max, avg, predict, time, step, median " << std::endl;
}


// C2: help cmd
/**output help for the specified command*/
void MerkelMain::helpCmd(std::vector<std::string>tokens)
{
    if (isTokensSecondElementValid(tokens[1]))  // to check whether the second element of tokens is valid; return true to implement the following 7 if statements
    {
        if (tokens[1] == "prod") 
        {
            std::cout << "prod -> ETH/BTC, DOGE/BTC, etc" << std::endl;
        }

        if (tokens[1] == "min") 
        {
            std::cout << "min ETH/BTC bid -> minimum ETH/BTC bid in current time step" << std::endl;
        }

        if (tokens[1] == "max") 
        {
            std::cout << "max ETH/BTC bid -> maximum ETH/BTC bid in current time step" << std::endl;
        }

        if (tokens[1] == "avg") 
        {
            std::cout << "avg ETH/BTC bid 10 -> average ETH/BTC bid over the last 10 time steps" << std::endl;
        }

        if (tokens[1] == "predict") 
        {
            std::cout << "predict max ETH/BTC bid -> The maximum ETH/BTC bid over the last 10 time steps was 20.1" << std::endl;
        }

        if (tokens[1] == "time") 
        {
            std::cout << "time -> 2020/03/17 17:01:24" << std::endl;
        }

        if (tokens[1] == "step") 
        {
            std::cout << "step -> now at 2020/03/17 17:01:30" << std::endl;
        }

        if (tokens[1] == "median") 
        {
            std::cout << "median ETH/BTC bid 10 -> the median of ETH/BTC bid over the last 10 time steps was 1.2" << std::endl;
        }
    }

    else
    {  // deal with illegal commands, such as "boy"
        throw std::invalid_argument("Bad Input From User; Please Type Like help max; For More Information About How To Use AdvisorBotPlease Type: help");
    }
}


// C3: prod
/** list available products*/
void MerkelMain::printKnownProducts()
{
    // call the method getKnownProducts of the object orderBook, and store the results in vector products
    std::vector<std::string>products = orderBook.getKnownProducts();

    // traverse the vector products to print out every element
    for (std::string const&i : products)
    {
        // print the last element of products without the comma(,)
        if(i == products[products.size() - 1])
        {
            std::cout << i << std::endl;
        }
        // print all elements except the last element ended with comma(,)
        else
        {
            std::cout << i << ",";
        }
    }
   
}



// C4: min
/** check whether the input tokens is valid; If valid, print min*/
void MerkelMain::printMin(std::vector<std::string>tokens)
{
    std::string cmdCommand = tokens[0];
    std::string cmdProduct = tokens[1];   
    std::string cmdTypeString = tokens[2];
    OrderBookType cmdType =  OrderBookEntry::stringToOrderBookType(tokens[2]); //cmdType is OrderBookType


    if ((tokens.size() == 3) &&   //only legal input of tokens will trigger further processing in order to save computing power, and improve user experience 

        (cmdCommand == "min") &&   
        isProductValid(cmdProduct) &&  
        isAskOrBid(cmdTypeString))
    {
        // return a filtered vector of OrderBookEntry objects
        std::vector<OrderBookEntry> entries = orderBook.getOrders(cmdType,      
                                                                  cmdProduct, 
                                                                  currentTime);
        // calcultae the min value using getLowPrice method                                                          
        double min = OrderBook::getLowPrice(entries);
        std::cout << "The min " << cmdTypeString << " for " <<  cmdProduct << " is " << min << std::endl;
    }

    else 
    { // deal with illegal commands, such as "min boy girl"
        throw std::invalid_argument("Bad Input From User; Please Type Like min ETH/BTC ask; For More Information Please Type: help min");
    }
}




// C5: max
/** check whether the input tokens is valid; If valid, print max*/
void MerkelMain::printMax(std::vector<std::string>tokens)
{
    std::string cmdCommand = tokens[0];
    std::string cmdProduct = tokens[1];   
    std::string cmdTypeString = tokens[2];
    OrderBookType cmdType =  OrderBookEntry::stringToOrderBookType(tokens[2]); //cmdType is OrderBookType

    if ((tokens.size() == 3) &&    //only legal input of tokens will trigger further processing in order to save computing power, and improve user experience
        (cmdCommand == "max") &&    
        isProductValid(cmdProduct) && 
        isAskOrBid(cmdTypeString))
    {
        // return a filtered vector of OrderBookEntry objects
        std::vector<OrderBookEntry> entries = orderBook.getOrders(cmdType, 
                                                                  cmdProduct, 
                                                                  currentTime);
        // calcultae the min value using getLowPrice method           
        double max = OrderBook::getHighPrice(entries);
        std::cout << "The max " << cmdTypeString << " for " << cmdProduct << " is " << max << std::endl;
    }

    else 
    { // deal with illegal commands, such as "max boy girl"
        throw std::invalid_argument("Bad Input From User; Please Type Like max ETH/BTC bid; For More Information Please Type: help max");
    }
}




// C6: avg
/** check whether the input tokens is valid; If valid, print average*/
void MerkelMain::printAvg(std::vector<std::string>tokens)
{ 
    std::string cmdCommand = tokens[0];
    std::string cmdProduct = tokens[1];   
    std::string cmdTypeString = tokens[2];
    OrderBookType cmdType =  OrderBookEntry::stringToOrderBookType(tokens[2]); //cmdType is OrderBookType
    int cmdNumOfTimestamps = std::stoi(tokens[3]); 

    if ((tokens.size() == 4) &&     //only legal input of tokens will trigger further processing in order to save computing power, and improve user experience
        (cmdCommand == "avg") &&   
        isProductValid(cmdProduct) &&   
        isAskOrBid(cmdTypeString) && 
        isTimestampsEnough(cmdNumOfTimestamps))
    {
        // calcultae the min value using getLowPrice method  
        double avg = MerkelMain::getAvg(cmdType, cmdProduct, cmdNumOfTimestamps);
        std::string timestepsOrTimestep;

        if(cmdNumOfTimestamps == 1) // if there is just one time point, we set the string timestepsOrTimestep as singular, that is "timestep"
        {
            timestepsOrTimestep = "timestep";
        }
        else
        {     // else set the string timestepsOrTimestep plural, that is timesteps
            timestepsOrTimestep = "timesteps";
        }
        std::cout << "The average " << cmdProduct << " " <<cmdTypeString << " price over the last " << cmdNumOfTimestamps << " " << timestepsOrTimestep << " was " << avg << std::endl;

    }

    else 
    { // deal with illegal commands, such as "avg boy girl"
        throw std::invalid_argument("Bad Input From User; Please Type Like avg ETH/BTC ask 2; For More Information Please Type: help avg");
    }
}



/** return average ask or bid for the sent product over the sent number of time steps*/
double MerkelMain::getAvg(OrderBookType cmdType, std::string cmdProduct, int cmdNumOfTimestamps)
{
    double OrderFilteredSum = 0;
    int OrderFilteredSize = 0;
    double avg;

    std::vector<OrderBookEntry> ordersFiltered;
    std::vector<std::string> foundedTimestamps = findRequiredTimestamps(cmdNumOfTimestamps); // based on the number of timestamps, we find the array of all required timestamps
   
    for(int i = 0; i < cmdNumOfTimestamps;i++)
    {
        ordersFiltered = orderBook.getOrders(cmdType, cmdProduct, foundedTimestamps[i]); // ordersFiltered is a vector of filtered orders
        OrderFilteredSum = OrderFilteredSum + OrderBook::getOrdersPriceSum(ordersFiltered); // sum algorithm
        OrderFilteredSize = OrderFilteredSize + ordersFiltered.size(); // also add up the size of each ordersFiltered
    }

    avg = OrderFilteredSum / OrderFilteredSize; 
    return avg;

}


// C7: predict
/** predict max or min ask or bid for the sent product for the next timestep*/
void MerkelMain::printPredict(std::vector<std::string>tokens)
{
    std::string cmdCommand = tokens[0];
    std::string cmdMaxOrMin = tokens[1];   
    std::string cmdProduct = tokens[2];   
    std::string cmdTypeString = tokens[3];
    OrderBookType cmdType =  OrderBookEntry::stringToOrderBookType(tokens[3]); //cmdType is OrderBookType

    if ((tokens.size() == 4) &&  //only legal input of tokens will trigger further processing in order to save computing power, and improve user experience
        (cmdCommand == "predict") && 
        isMaxOrMin(cmdMaxOrMin) &&   
        isProductValid(cmdProduct) && 
        isAskOrBid(cmdTypeString) )  
    {
        std::vector<double> yValues; 
        std::vector<int> xValues;
        std::vector<double> formula;

        double x; 
        double y;
        int index;
        int NumOfTimestamps;
        
        NumOfTimestamps = index + 1 ;
        x= 5 * NumOfTimestamps; 
        yValues = getYValues(cmdMaxOrMin, cmdProduct, cmdType); // values on y-axis
        xValues = getXValues(NumOfTimestamps); // values on x-axis

        index = findIndexOfCurrentTimestamp();
        if(index == 0) // if there is only one time point, we make the only y value in the array yValue equal to the spredict value of the next time step's max/min product ask/bid
        {
            std::cout << "The predict " << cmdMaxOrMin << " " << cmdProduct << " " << cmdTypeString << " price for the next time step is " << yValues[0] << std::endl;
        }
       
        else
        {
            formula = fitLinearRegression(yValues, xValues); // using linear regression for prediction (its formula is y = mx + b)
            y = getPredict(formula, x); //call getPredict function to predict the value of y based on the value of x
            std::cout << "The predict " << cmdMaxOrMin << " " << cmdProduct << " " << cmdTypeString << " price for the next time step is " << y << std::endl;
        }
    }

    else
    { // deal with illegal commands, such as "predict max boy girl"
        throw std::invalid_argument("Bad Input From User; Please Type Like predict max ETH/BTC ask; For More Information Please Type: help predict");
    }
}




/** return the values of y-axis and store them in a vector*/
std::vector<double> MerkelMain::getYValues(std::string cmdMaxOrMin, std::string cmdProduct, OrderBookType cmdType)
{
    int index;
    int NumOfTimestamps;
    double extremum;
    std::vector<double> extremums;
    std::vector<OrderBookEntry> ordersFiltered;
   
    index = findIndexOfCurrentTimestamp();
    NumOfTimestamps = index + 1 ;
    std::vector<std::string> foundedTimestamps = findRequiredTimestamps(NumOfTimestamps);

    for(int i = 0; i < NumOfTimestamps;i++)
    {
        ordersFiltered = orderBook.getOrders(cmdType, cmdProduct, foundedTimestamps[i]);
        extremum = useMaxOrMin(cmdMaxOrMin, ordersFiltered);
        extremums.push_back(extremum);
    }
    return extremums;

}


/** return the values of x-axis and store them in a vector*/
std::vector<int> MerkelMain::getXValues(int NumOfTimestamps)
{
    std::vector<int> xValues;
    int xValue;
    //the for loop is to generate the vector xValues with starting value xValue as 0;
    for(int i = 0; i < NumOfTimestamps; i++)
    {
        xValue = i * 5;
        xValues.push_back(xValue);
    }
   
    return xValues;
   
}


/** return max or min based on the input of "max" or "min*/
double MerkelMain::useMaxOrMin(std::string cmdMaxOrMin, std::vector<OrderBookEntry> ordersFiltered)
{
    double min;
    double max;
    
    // if-else condition is to judge whether use getHighPrice() or getLowPrice() based on the input of "max" or "min"
    if(cmdMaxOrMin == "max")
    {
        max = OrderBook::getHighPrice(ordersFiltered);  //calculate max prices
        return max;
    }
    
    else
    {
        min = OrderBook::getLowPrice(ordersFiltered); //calculate min prices
        return min;
    }
}


/** return a vector containing slope and y-intercept based on the formula of linear regression*/
std::vector<double> MerkelMain::fitLinearRegression(std::vector<double>yValues, std::vector<int>xValues)
{
    double sumX = 0;
    double sumY = 0;
    double sumXY = 0;
    double sumX2 = 0;
    double m;
    double b;
    int length = yValues.size();
    std::vector<double> formula;

    for(int i = 0; i < yValues.size(); i++)
    {
        sumX += xValues[i];
        sumY += yValues[i];
        sumXY += xValues[i] * yValues[i];
        sumX2 += xValues[i] * xValues[i];
    }

    m = (length * sumXY - sumX * sumY) / (length * sumX2 - sumX * sumX); // linear regression rule to calculate m (namely slope)
    b = (sumY - m * sumX) /length; // linear regression rule to calculate b (namely y-intercept)

    formula.push_back(m); //the first element of vector formula is m, which is slope
    formula.push_back(b); //the second element of vector formula is m, which is y-intercept

    return formula;
}


/** calculate the predicted value of y based on linear regression formula and x value*/
double MerkelMain::getPredict(std::vector<double> formula, double x)
{
    double m;
    double b;
    double predictedValue;
 

    m = formula[0]; // m is the slope
    b = formula[1]; // b is the y-intercept

    predictedValue = m * x + b; //use linear regression formula: y = mx + b
    return predictedValue;
}


/** return index of current timestamp on the vector of all known timestamps*/
int MerkelMain::findIndexOfCurrentTimestamp()
{  
    // knownTimestamps is a vector of all known timestamps in database
    std::vector<std::string> knownTimestamps =orderBook.getKnownTimestamps();

    for(int i = 0; i < knownTimestamps.size();i++)
    {
        if(currentTime == knownTimestamps[i])
        {
           return i;
        }
        
    }
    return -1;
} 
    // Finding an element called currentTime in vector using STL Algorithm std::find() 
    // std::vector<std::string>::iterator it = std::find(knownTimestamps.begin(), knownTimestamps.end(), currentTime);

    // // If element is found then we can get its index from the iterator
    // IndexOfCurrentTimestamp = std::distance(knownTimestamps.begin(), it);
    
    // return IndexOfCurrentTimestamp;
      
    

/** return all required timestamps based on the input, which is number of timestamps*/
std::vector<std::string> MerkelMain::findRequiredTimestamps(int timesteps)
{
    std::vector<std::string> foundedTimestamps;
    // int index = findIndexOfCurrentTimestamp();
    std::vector<std::string> knownTimestamps = orderBook.getKnownTimestamps();
    
    for(int j = 0; j < timesteps; j++)
    {
        foundedTimestamps.push_back(knownTimestamps[j]); // put all required timestamps on the vector founded Timestamps
    }  

    return foundedTimestamps;
}


/** return an array of prices based on the inputs: orderBookType(ask or bid), product type, number of timesteps*/
std::vector<double> MerkelMain::getPrices(OrderBookType cmdType, std::string cmdProduct, int cmdNumOfTimestamps)
{
    double median;
    std::vector<double> prices;

    std::vector<OrderBookEntry> ordersFiltered;
    std::vector<std::string> foundedTimestamps = findRequiredTimestamps(cmdNumOfTimestamps);
   
    for(int i = 0; i < cmdNumOfTimestamps;i++)
    {
        ordersFiltered = orderBook.getOrders(cmdType, cmdProduct, foundedTimestamps[i]);
        for(OrderBookEntry& e : ordersFiltered)
        {
            prices.push_back(e.price);
        }
    }
    return prices;
}


// C8: time
/** state current time in dataset, i.e. which timeframe are we looking at*/
void MerkelMain::printCurrentTime()
{
    std::cout << currentTime << std::endl; // current time is a private varialble in class MerkelMain
}

//C9: step
/** print the next time step*/
void MerkelMain::gotoNextTimeframe()
{
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "Now at " << currentTime << std::endl;
}



// Task 2: implement your own command - median ETH/BTC ask 10
/** check whether the input tokens is valid; If valid, print median*/
void MerkelMain::printMedian(std::vector<std::string>tokens)
{
   
    std::string cmdCommand = tokens[0];
    std::string cmdProduct = tokens[1];   
    std::string cmdTypeString = tokens[2];
    OrderBookType cmdType =  OrderBookEntry::stringToOrderBookType(tokens[2]);
    int cmdNumOfTimestamps = std::stoi(tokens[3]);


    if ((tokens.size() == 4) &&    //only legal input of tokens will trigger further processing in order to save computing power, and improve user experience
        (cmdCommand) == "median" && 
        isProductValid(cmdProduct) && 
        isAskOrBid(cmdTypeString) && 
        isTimestampsEnough(cmdNumOfTimestamps)) 
    {
        double med;
        std::string timestepsOrTimestep;
        std::vector<double> prices = MerkelMain::getPrices(cmdType, cmdProduct, cmdNumOfTimestamps);
        std::sort(prices.begin(), prices.end());
        med = MerkelMain::getMedianNumber(prices, prices.size());


        if(cmdNumOfTimestamps == 1)  // if there is just one time point, we set the string timestepsOrTimestep as singular, that is "timestep"
        {
            timestepsOrTimestep = "timestep";
        }

        else
        {  // else set the string timestepsOrTimestep plural, that is timesteps
            timestepsOrTimestep = "timesteps";
        }

        std::cout << "The median " << cmdProduct << " " <<cmdTypeString << " price over the last " << cmdNumOfTimestamps << " " << timestepsOrTimestep << " was " << med << std::endl;
    }

    else 
    { // deal with illegal commands, such as "median boy girl life"
        throw std::invalid_argument("Bad Input From User; Please Type Like median ETH/BTC bid 10; For More Information Please Type: help median");       
    }
}

/** return the value of median based on the definition of median in math*/
double MerkelMain::getMedianNumber(std::vector<double> theArray , int size)
{
    double median;
    int mid = floor((size - 1)/2);
    if(size % 2 != 0) // check whether the size of the array is odd
    {
        median = theArray[mid]; // the definition of median in math when the size of the array is odd
        return median;
    }

    else
    {    // check whether the size of the array is even
        median = (theArray[mid]+  theArray[mid+1])/2; // the definition of median in math when the size of the array is even
        return median;
    }
}



/** a boolean function to check whether the input of product is valid*/
bool MerkelMain::isProductValid(std::string type)
{
    std::vector<std::string> products = orderBook.getKnownProducts();
    // traverse the vector products to search whether type is in the vector 
    for (std::string const& e : products)
    {
       if(e == type)
       {
           return true;
       }
    }
    return false;
}

/** a boolean function to check whether the input of ask/bid is valid*/
bool MerkelMain::isAskOrBid(std::string type)
{
    return(type == "ask" || type == "bid"); 
}

/** a boolean function to check whether the input of max/min is valid*/
bool MerkelMain::isMaxOrMin(std::string type)
{
    return(type == "max" || type == "min"); 
}

/** a boolean function to check whether the vector of timestamps has an enough size to call for calculating average/median "*/
bool MerkelMain::isTimestampsEnough(int NumOfTimestamps)
{
    if(NumOfTimestamps < 0)
    {
        return false;
    }

    int index = findIndexOfCurrentTimestamp();
    if(index + 1 - NumOfTimestamps >= 0)
    {
        return true;
    }

    else
    {
        return false;
    }
}

/** a boolean function to check whether the second element is valid*/
bool MerkelMain::isTokensSecondElementValid(std::string tokensTwo)
{
    std::vector<std::string> validElements;
    validElements = {"prod", "min", "max", "avg", "predict", "time", "step", "median"}; 

      // traverse the vector validElements to search whether tokensTwo is in the vector;
    for (std::string const&i : validElements)
    {
        if(i == tokensTwo)
        {
            return true; 
        }
    }
    return false;
}




 
/** get User's input and store the result in variable userOption*/
std::string MerkelMain::getUserOption()
{
    std::string userOption;
    std::string line;
    std::getline(std::cin, line);
    if(!isWhitespace(line)) // call the boolean function to check whether it is Whitespace
    {
        userOption = line;
        std::cout << "You chose: " << userOption << std::endl;
        return userOption;
    
    }
    else
    {
        userOption = "empty"; //if it is Whitespace, then store the string "empty" in private variable userOption and return userOption for further process in function processUserOption
        return userOption;
    }
    
}


/** A boolean function to check whether it is Whitespace.*/
bool MerkelMain::isWhitespace(std::string s)
{
    for(int i = 0; i < s.length(); i++){
        if(!std::isspace(s[i]))
            return false;
    }
    return true;
}

/**call main methods of app functionalities by processing the private variable userOption*/
void MerkelMain::processUserOption(std::string userOption)
{
    std::vector<std::string>tokens = CSVReader::tokenise(userOption, ' ');
    std::string cmd = tokens[0]; //initial tokenise userOption to find the content of tokens[0] and store it in string variable cmd

    try{

        if (cmd == "help" && tokens.size() == 1)  // if the size is 1 and the variable cmd is "help", call function printHelp()
        {
            printHelp();
        }

        else if (cmd == "help" && tokens.size() == 2)  // if the size is 2 and the variable cmd is "help", call function printCmd()
        {
            helpCmd(tokens);
        }

        else if (cmd == "prod" && tokens.size() == 1) // if the variable cmd is "prod", call function printKnownProducts(); the strict check of inputs will be processed on printKnownProducts
        {
            printKnownProducts();
        }

        else if (cmd == "min") 
        {
            printMin(tokens);
        }

        else if (cmd == "max") 
        {
            printMax(tokens);
        }

        else if (cmd == "avg") 
        {
            printAvg(tokens);
        }       

        else if (cmd == "predict") 
        {
            printPredict(tokens);
        }   

        else if (cmd == "time" && tokens.size() == 1) 
        {
            printCurrentTime();
        }   

        else if (cmd == "step" && tokens.size() == 1) 
        {
            gotoNextTimeframe();
        }  

        else if (cmd == "median") 
        {
            printMedian(tokens);
        }    

        else if (userOption == "empty") //if the input is Whitespace, the stored string "empty" in private variable userOption will make this if-statement true and print out messages to alert user
        {
            std::cout << "You typed whitespace; For More Information About How To Use AdvisorBot, Please Type: help" << std::endl;
        }

        else
        {
            std::cout << "Bad Input for commands! For More Information About How To Use AdvisorBot, Please Type: help " << std::endl;
        }
    }
    catch (const std::exception& e)  // print the errors from main methods of app functionalities, e.g., functions like helpCmd(),  printMin(), printPredict, printMedian etc.
        {
            std::cout << e.what() << std::endl;
        }   
} 


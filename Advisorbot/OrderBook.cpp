#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>


/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}



/** return vector of all know products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    // if the size of vector knownProducts is not empty, which means there is/are already some elements inside
    if(knownProducts.size() != 0)
    {
        return knownProducts; //return knownProducts directly in order to save computing power while calling the knownProducts method
    }



    std::vector<std::string> products;

    std::map<std::string,bool> prodMap;  // use dictionary technique to map string : bool 

    for (OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }
    
    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    // store the results of products 
    knownProducts = products;
   
    return products;
}


/** calculate the sum of prices of objects*/
double OrderBook::getOrdersPriceSum(std::vector<OrderBookEntry> orders)
{
    double sum = 0;
    for(OrderBookEntry& e : orders)
    {
        sum = sum + e.price;
    }

    return sum;
}


/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, 
                                        std::string product, 
                                        std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == type &&   // filter the vector oders to generate orders_sub with 3 conditions, namely type, product, and timestamp(i.e., Number of Timestamps)
            e.product == product && 
            e.timestamp == timestamp )
            {
                orders_sub.push_back(e);
            }
    }
    return orders_sub;
}



/** calculate the max price*/
double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max)max = e.price;
    }
    return max;
}


/** calculate the min price*/
double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price < min)min = e.price;
    }
    return min;
}


/** calculate the earliest time because the csv file is already sorted by time*/
std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}


/** return the next timestamp */
std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp) 
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}


/** return all known timestamps of the csv file */
std::vector<std::string> OrderBook::getKnownTimestamps()
{
    //if the size of vector knownTimestamps is not empty, which means there is/are already some elements inside
    if(knownTimestamps.size() != 0)
    {
        return knownTimestamps;      // return knownTimestamps directly in order to save computing power while calling the getKnownTimestamps method
    }

    std::vector<std::string> timestamps;
    std::map<std::string,bool> timestampsMap;

    for (OrderBookEntry& e : orders) // use dictionary technique to map string : bool 
    {
        timestampsMap[e.timestamp] = true;
    }
    
    // now flatten the map to a vector of strings
    for (auto const& e : timestampsMap)
    {
        timestamps.push_back(e.first);
    }

    knownTimestamps = timestamps;
    return timestamps;
}









#include "../327_proj3_test/includes/FileIO.h"
#include "../327_proj3_test/includes/constants.h"
#include "../327_proj3_test/includes/StringParserClass.h"
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;
using namespace KP_StringParserClass;

//dont forget to initialize member variables
StringParserClass::StringParserClass() {
	//set default construct
	pStartTag = NULL;
	pEndTag = NULL;
	areTagsSet = false;
}

//call cleanup to release any allocated memory
StringParserClass::~StringParserClass() {
	cleanup();
}

//these are the start tag and the end tags that we want to find,
//presumably the data of interest is between them, please make a
//COPY of what pStartTag and pEndTag point to.  In other words
//DO NOT SET pStartTag = pStart
//returns:
//SUCCESS
//ERROR_TAGS_NULL if either pStart or pEnd is null
int StringParserClass::setTags(const char *pStart, const char *pEnd) {
	//Quickly end the method if receiving NULL
	if (pStart == NULL || pEnd == NULL) {
		return ERROR_TAGS_NULL;
	}
	//get integers with the size of the pointers to be copied
	int slength = strlen(pStart);
	int elength = strlen(pEnd);

	//Clear it and make it into a new char pointer that has the same size as the data about to be copied
	pStartTag = new char[slength - 1];
	pEndTag = new char[elength - 1];

	//Copy the data from the first pointer to our global variable
	strcpy(pStartTag, pStart);
	strcpy(pEndTag, pEnd);

	return SUCCESS;
}

//First clears myVector
//going to search thru pDataToSearchThru, looking for info bracketed by
//pStartTag and pEndTag, will add that info only to myVector
//returns
//SUCCESS  finished searching for data between tags, results in myVector (0 or more entries)
//ERROR_TAGS_NULL if either pStart or pEnd is null
//ERROR_DATA_NULL pDataToSearchThru is null
int StringParserClass::getDataBetweenTags(char *pDataToSearchThru,
		std::vector<std::string> &myVector) {
	//First clears myVector
	myVector.clear();

	if (pStartTag == NULL || pEndTag == NULL) {
		return ERROR_TAGS_NULL;
	}
	if (pDataToSearchThru == NULL) {
		return ERROR_DATA_NULL;
	}

	//Gotta make the scope of this big
	int index = strlen(pDataToSearchThru);
	int i = 0;
	bool lookingForStartTag = true;
	int returnVal;
	string returnString = "";
	while (index != 0) {

		if (pDataToSearchThru[i] == '<') {
			char *start = &pDataToSearchThru[i];
			char *end = &pDataToSearchThru[i];		//holder variable

			if (lookingForStartTag) {
				returnVal = findTag(pStartTag, start, end);
			} else {
				returnVal = findTag(pEndTag, start, end);
			}

			//start doing real work here
			if (returnVal == SUCCESS) {
				if (!lookingForStartTag) {
					myVector.push_back(returnString);
					lookingForStartTag = true;
					returnString = "";
				} else {		//not looking for start tag
					lookingForStartTag = false;
					i += strlen(pStartTag);
				}
			}
		}

		if(!lookingForStartTag){
			returnString += pDataToSearchThru[i];
		}
		i++;
		index--;
	}

	return SUCCESS;
}

//Function (and everything below here) is default private because of .h file???
void StringParserClass::cleanup() {
	//Dereference and make sure no dangling pointers
	if (areTagsSet) {
		delete pStartTag;
		delete pEndTag;
	}
	pStartTag = NULL;
	pEndTag = NULL;
	areTagsSet = false;
}

//Searches a string starting at pStart for pTagToLookFor
//returns:
//SUCCESS  found pTagToLookFor, pStart points to beginning of tag and pEnd points to end of tag
//FAIL did not find pTagToLookFor and pEnd points to 0
//ERROR_TAGS_NULL if either pStart or pEnd is null
int StringParserClass::findTag(char *pTagToLookFor, char *&pStart,
		char *&pEnd) {
	if (pStartTag == NULL || pEndTag == NULL) {
		return ERROR_TAGS_NULL;
	}

	int startindex = strlen(pTagToLookFor);
	int x = 0;
	bool startHere = true;
//loops through the chars
	while (startindex != 0) {
		//validate that the current char is equal to the same char in the startTag
		if (pStart[x] != pTagToLookFor[x]) {
			//if not then break out of loop and dont start looking for values here
			startHere = false;
			break;
		}
		x++;
		startindex--;
	}
	if (startHere) {
		//THE CODE HERE SEEMS TO WORK OF THE SET TAGS WORKS PROPERLY
		pEnd = &pStart[x];
		return SUCCESS;
	}
	return FAIL;
}

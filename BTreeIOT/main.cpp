#include <iostream>
#include <string>

#include "Support.h"
#include "BTree.h"

struct OPTIONS Opts;

void Usage()
{
	std::cout << "Usage: BPlusTree [options]" << std::endl;
	std::cout << std::endl;
	std::cout << "--order number" << std::endl;
}

int parse_options(int argc, char* argv[])
{
	int nRes = 0;
	try {
		if (!argv || argc == 0)
			throw std::exception("Please provide arguments");

		int nArgs = 1;
		if (argv[nArgs] && _stricmp(argv[nArgs++], "--order") == 0) {
			Opts.nOrder = atoi(argv[nArgs++]);
		}

		if (argv[nArgs] && _stricmp(argv[nArgs++], "--select") == 0) {				/* select command */
			Opts.eCommand = eSelect;
			++argv;
			/* at the moment we only provide key */
			Opts.nKEY = atoi(argv[nArgs++]);
		}

		if (argv[nArgs] && _stricmp(argv[nArgs++], "--delete") == 0) {				/* delete command */
			Opts.eCommand = eDelete;
			++argv;
			/* at the moment we only provide key */
			Opts.nKEY = atoi(argv[nArgs++]);
		}
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		Usage();
	}
	return nRes;
}

int main(int argc, char* argv[])
{
	if (parse_options(argc, argv))
		return 0;


	CBTree cBTreeObj;

	if (Opts.nOrder < 3 || Opts.nOrder > 20) {
		std::cerr << "Order not supported!!!" << std::endl;
		return 0;
	}

	cBTreeObj.SetOrder(Opts.nOrder);

	while (true) {
		if (Opts.eCommand == eNone) {
			std::cout << "Available Commands" << std::endl;
			std::cout << "insert key" << std::endl;
			std::cout << "delete key" << std::endl;
			std::cout << "select key" << std::endl;
			std::cout << "print" << std::endl;
			std::cout << "exit" << std::endl;
			std::cout << std::endl;

			std::string csCommand;
			//std::cin.ignore();
			std::getline(std::cin, csCommand);
			if (_strnicmp(csCommand.c_str(), "select", strlen("select")) == 0) {
				/* select command */
				size_t nIndex = csCommand.rfind(" ");
				Opts.nKEY = atoi(csCommand.substr(nIndex).c_str());
				Opts.eCommand = eSelect;
			}
			else if (_strnicmp(csCommand.c_str(), "delete", strlen("delete")) == 0) {
				/* delete command */
				size_t nIndex = csCommand.rfind(" ");
				Opts.nKEY = atoi(csCommand.substr(nIndex).c_str());
				Opts.eCommand = eDelete;
			}
			else if (_strnicmp(csCommand.c_str(), "insert", strlen("insert")) == 0) {
				/* insert command */
				size_t nIndex = csCommand.rfind(" ");
				Opts.nKEY = atoi(csCommand.substr(nIndex).c_str());
				Opts.eCommand = eInsert;
			}
			else if (_strnicmp(csCommand.c_str(), "print", strlen("print")) == 0) {
				Opts.eCommand = ePrint;
			}
			else if (_strnicmp(csCommand.c_str(), "exit", strlen("exit")) == 0)
				break;
			else {
				std::cerr << "Invalid command" << std::endl;
				Usage();
			}
		}

		if (Opts.eCommand == eInsert)					/* insert command */
			cBTreeObj.Insert(Opts.nKEY, Opts.nKEY);
		else if (Opts.eCommand == eDelete)				/* delete command */
			cBTreeObj.Delete(Opts.nKEY);
		else if (Opts.eCommand == eSelect)				/* select command */
			cBTreeObj.Find(Opts.nKEY);
		else if (Opts.eCommand == ePrint)
			cBTreeObj.PrintTree();

		Opts.eCommand = eNone;							/* reset the command, to get next command */

	}

	return 0;
}
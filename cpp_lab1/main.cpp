#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>


std::unordered_set<std::string> types{ "char", "bool", "double", "int", "long", "short", "float" };
std::unordered_set<std::string> operations{ "/", "*", "+", "-" };


int main(int argc, char** argv)
{
	std::string config_file = "config.txt";
	if (argc >= 2)
		config_file = argv[1];

	std::string user_types_str;
	std::vector<std::string> user_types;

	std::string user_operations_str;
	std::vector<std::string> user_operations;

	std::string user_initial_val_str;
	std::vector<std::string> user_initial_val;

	int number_of_var;

	namespace po = boost::program_options;
	// setup the program options
	po::options_description desc("Allowed options");
	// clang-format off
	desc.add_options()
		("help", "help message")
		("variable_types", po::value<std::string>(&user_types_str)->default_value("int"), "which types to use(specify \"int\", \"int double\", etc)")
		("operations", po::value<std::string>(&user_operations_str)->default_value("+"), "which operations to use(specify \"+\", \"+ -\", etc)")
		("intial_values", po::value<std::string>(&user_initial_val_str)->default_value("0 0"), "which types to use(specify \"0\", \"0 0.0\", etc)")
		//("number_of_variables_for_each_type", po::value<int>(&number_of_var)->default_value(2), "numbers of variables with the same type")
		;

	std::ifstream input_file(config_file);

	// clang-format on
	po::variables_map vm;
	try {
		po::store(po::parse_config_file(input_file, desc), vm);
		po::notify(vm);
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}
	boost::split(user_types, user_types_str, boost::is_any_of(" "));
	boost::split(user_initial_val, user_initial_val_str, boost::is_any_of(" "));

	size_t number_of_types = user_types.size();
	for (size_t i = 0; i < user_types.size(); i++)
	{
		const std::string& type = user_types[i];

		if (types.find(type) == types.end() && type != "unsigned" && type != "signed")
		{
			std::cout << type + "is invalid type, please try again\n";
			return -1;
		}
		if ((type == "unsigned" || type == "signed") && i != user_types.size() - 1)
		{

			const std::string& next_type = user_types[i + 1];
			if (types.find(next_type) == types.end() && next_type != "unsigned" && next_type != "signed")
			{
				std::cout << next_type + " is invalid type, please try again\n";
				return -1;
			}
			user_types[i] += " " + next_type;
			user_types.erase(user_types.begin() + i + 1);
			number_of_types--;
		}
	}


	boost::split(user_operations, user_operations_str, boost::is_any_of(" "));
	for (size_t i = 0; i < user_operations.size(); i++)
	{
		const std::string& op = user_operations[i];

		if (operations.find(op) == operations.end())
		{
			std::cout << op + "is invalid operation, please try again\n";
			return -1;
		}
	}

	if (user_types.size() != user_initial_val.size()) 
	{
		std::cout << "Not enough initial values!\n";
		return -1;
	}
		

	std::ofstream output_file("foo.cpp");

	int counter = 0;
	output_file << "#include <iostream>\n\n";
	output_file << "void foo()\n{ \n";
	for (auto& type : user_types)
	{


		output_file << "\t" + type + " x" + std::to_string(counter) + " = " + user_initial_val[counter] +
									", y" + std::to_string(counter) + " = " + user_initial_val[counter] + "; \n";
		for (auto& op : user_operations)
		{
			output_file << "\tstd::cout << \"(" + type + ") " + user_initial_val[counter] + " " + op
										 + " (" + type + ") " + user_initial_val[counter] + " = \" ";
			output_file << "<< x" + std::to_string(counter) + " " + op + " y" + std::to_string(counter) + " << \"\\n\";";
			output_file << "\n";
		}
		counter++;
	}
	output_file << "}";

	return 0;
}
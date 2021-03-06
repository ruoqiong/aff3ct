#include "Tools/Arguments/Types/File_system/File_system.hpp"

#include "Argument_map_value.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

bool Argument_map_value
::exist(const Argument_tag &tags) const
{
	return (this->find(tags) != this->end());
}

int Argument_map_value
::to_int(const Argument_tag &tags) const
{
	try
	{
		return std::stoi(this->at(tags));
	}
	catch (std::exception&)
	{
		return 0;
	}
}

float Argument_map_value
::to_float(const Argument_tag &tags) const
{
	try
	{
		return std::stof(this->at(tags));
	}
	catch (std::exception&)
	{
		return 0.0f;
	}
}

std::string Argument_map_value::
to_file(const Argument_tag &tags) const
{
	if (Is_file::check(this->at(tags)))
		return this->at(tags);
	else
	{
		auto new_str = modify_path<Is_file>(this->at(tags));
		if (new_str.empty())
			return this->at(tags);
		else
			return new_str;
	}
}

std::string Argument_map_value::
to_folder(const Argument_tag &tags) const
{
	if (Is_folder::check(this->at(tags)))
		return this->at(tags);
	else
	{
		auto new_str = modify_path<Is_folder>(this->at(tags));
		if (new_str.empty())
			return this->at(tags);
		else
			return new_str;
	}
}

std::string Argument_map_value::
to_path(const Argument_tag &tags) const
{
	if (Is_path::check(this->at(tags)))
		return this->at(tags);
	else
	{
		auto new_str = modify_path<Is_path>(this->at(tags));
		if (new_str.empty())
			return this->at(tags);
		else
			return new_str;
	}
}
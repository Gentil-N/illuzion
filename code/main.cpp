#include <iostream>
#include <vector>
#include <functional>
#include <filesystem>
#include "illuzion.hpp"

#define VERSION_MAJOR 1
#define VERSION_MINOR 1

bool HELP = false;
bool VERSION = false;
std::string HELP_BASE_TXT =
    "Warning : procs are treated in order. Inputs and outputs too.\n"
    "Example of arguments : --inputs=\"./pic1.png\",\"./pic2.png\" --outputs=\"./pic1_igr.png\",\"./pic2_igr.png\" --invert --grey-scale --resize=1200,800";
bool INPUT_TYPE_FOLDER = false;
ilzn::PicType OUTPUT_TYPE = ilzn::PicType::PNG;
std::vector<std::string> INPUTS;
std::vector<std::string> OUTPUTS;
std::vector<std::function<void(ilzn::Picture &picture)>> PROCS;

const char OPT_INIT_DELIMITER = '-';
const char OPT_ARGS_DELIMITER = ',';
const char OPT_ASSIGN_DELIMITER = '=';
const char ARG_MUL = 'x';
const char END_STRING = '\0';

std::vector<std::string> get_args_from_option(const std::string &thing, size_t option_size_name)
{
       if (thing.size() < option_size_name || thing[option_size_name] != OPT_ASSIGN_DELIMITER)
       {
              throw std::runtime_error("failed to locate assignment for the option : " + thing);
       }
       std::string args = thing.substr(option_size_name + 1, thing.size());
       size_t last_args_pos = 0;
       std::vector<std::string> args_splited;
       for (size_t pos = 0; pos <= args.size(); ++pos)
       {
              if (args[pos] == OPT_ARGS_DELIMITER || args[pos] == END_STRING)
              {
                     if (pos - last_args_pos > 0)
                     {
                            args_splited.push_back(std::string(args, last_args_pos, pos));
                     }
                     last_args_pos = pos + 1;
              }
       }
       return args_splited;
}

bool option_name_is_matching(const std::string &option, const std::string &match_thing)
{
       if (match_thing.size() < option.size() + 2 || match_thing[0] != OPT_INIT_DELIMITER || match_thing[1] != OPT_INIT_DELIMITER)
       {
              return false;
       }
       for (size_t i = 0; i < option.size(); ++i)
       {
              if (match_thing[i + 2] != option[i])
              {
                     return false;
              }
       }
       if (match_thing.size() > option.size() + 2 && match_thing[option.size() + 2] != OPT_ASSIGN_DELIMITER)
       {
              return false;
       }
       return true;
}

struct Option
{
       std::string name;
       std::string description;
       std::function<void()> fct;
};

struct OptionMultiArgs
{
       std::string name;
       std::string description;
       std::function<void(const std::vector<std::string> &)> fct;
};

std::vector<Option> OPTIONS =
    {
        {"help",
         "print help (what you're reading)",
         []() -> void {
                HELP = true;
         }},
        {"version",
         "print actual version",
         []() -> void {
                VERSION = true;
         }},
        {"input-type-folder",
         "inputs option become a folder that takes 1 arguments and the outputs option become the suffix for the outputs (do not enter specials characters for suffix like ?, \\...)",
         []() -> void {
                INPUT_TYPE_FOLDER = true;
         }},
        {"grey-scale",
         "(proc) get grey scale",
         []() -> void {
                PROCS.push_back(
                    [](ilzn::Picture &picture) -> void {
                           ilzn::Picture gs;
                           gs.alloc(picture);
                           ilzn::effect::grey_scale(picture, gs);
                           picture.copy(gs);
                    });
         }},
        {"invert",
         "(proc) invert colors",
         []() -> void {
                PROCS.push_back(
                    [](ilzn::Picture &picture) -> void {
                           ilzn::Picture ivn;
                           ivn.alloc(picture);
                           ilzn::effect::invert(picture, ivn);
                           picture.copy(ivn);
                    });
         }},
        {"flip-horizontal",
         "(proc) flip picture along its hoizontal line",
         []() -> void {
                PROCS.push_back(
                    [](ilzn::Picture &picture) -> void {
                           ilzn::Picture fh;
                           fh.alloc(picture);
                           ilzn::effect::flip_horizontal(picture, fh);
                           picture.copy(fh);
                    });
         }}};
std::vector<OptionMultiArgs> OPTIONS_MULTI_ARGS =
    {
        {"inputs",
         "give pictures for inputs",
         [](const std::vector<std::string> &args) -> void {
                INPUTS = args;
         }},
        {"outputs",
         "give names for outputs without extension name",
         [](const std::vector<std::string> &args) -> void {
                OUTPUTS = args;
         }},
        {"output-type",
         "specify type of picture for the output (one of these): .png .bmp .tga .jpg",
         [](const std::vector<std::string> &args) -> void {
                if (args.size() != 1)
                {
                       throw std::runtime_error("output type has too many arguments : given " + std::to_string(args.size()));
                }
                OUTPUT_TYPE = ilzn::get_picture_type_from_extension_name(args[0]);
         }},
        {"resize",
         "(proc) resize picture with these two arguments : width (xW to mul by W)), height (xH to mul by H)",
         [](const std::vector<std::string> &args) -> void {
                if (args.size() != 2)
                {
                       throw std::runtime_error("argument count is not valid : expected 2 but given " + std::to_string(args.size()));
                }
                size_t width = 0, height = 0;
                float mul_width = 0, mul_height = 0;
                if (args[0][0] == ARG_MUL)
                {
                       mul_width = std::stof(args[0].substr(1, args[0].size()));
                }
                else
                {
                       width = std::stoul(args[0]);
                }
                if (args[1][0] == ARG_MUL)
                {
                       mul_height = std::stof(args[1].substr(1, args[1].size()));
                }
                else
                {
                       height = std::stoul(args[1]);
                }
                PROCS.push_back(
                    [width, height, mul_width, mul_height](ilzn::Picture &picture) -> void {
                           ilzn::Picture rsz;
                           rsz.alloc(width + picture.width() * mul_width, height + picture.height() * mul_height);
                           ilzn::effect::resize(picture, rsz);
                           picture.copy(rsz);
                    });
         }}};

void parse_args(int argc, char *argv[])
{

       for (size_t i = 0; i < argc; ++i)
       {
              std::string thing(argv[i]);
              bool matched = false;
              for (Option &option : OPTIONS)
              {
                     if (option_name_is_matching(option.name, thing))
                     {
                            option.fct();
                            matched = true;
                     }
              }
              for (OptionMultiArgs &option_multi_args : OPTIONS_MULTI_ARGS)
              {
                     if (option_name_is_matching(option_multi_args.name, thing))
                     {
                            option_multi_args.fct(get_args_from_option(thing, option_multi_args.name.size() + 2));
                            matched = true;
                     }
              }
              if (!matched)
              {
                     throw std::runtime_error("option unknown : " + thing);
              }
       }
}

void print_help()
{
       std::cout << "-------------------------------" << std::endl;
       std::cout << HELP_BASE_TXT << std::endl;
       std::cout << "List of options : " << std::endl;
       for (Option &option : OPTIONS)
       {
              std::cout << "       --" << option.name << " : " << option.description << std::endl;
       }
       for (OptionMultiArgs &option_multi_args : OPTIONS_MULTI_ARGS)
       {
              std::cout << "       --" << option_multi_args.name << "=args : " << option_multi_args.description << std::endl;
       }
       std::cout << "-------------------------------" << std::endl;
}

void process()
{
       if (VERSION)
       {
              std::cout << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;
       }
       if (HELP)
       {
              print_help();
       }
       if (INPUTS.empty())
       {
              return;
       }
       if (INPUT_TYPE_FOLDER)
       {
              if (INPUTS.size() != 1 || OUTPUTS.size() != 1)
              {
                     throw std::runtime_error("input or output count is different from 1");
              }
              std::string dir = INPUTS[0];
              std::string output_suffix = OUTPUTS[0];
              if (!std::filesystem::is_directory(dir))
              {
                     throw std::runtime_error("input given is not a directory");
              }
              INPUTS.clear();
              OUTPUTS.clear();
              for (const auto &file : std::filesystem::directory_iterator(dir))
              {
                     size_t pos = 0;
                     if (ilzn::is_a_picture(file.path(), pos))
                     {
                            std::string file_path = file.path();
                            INPUTS.push_back(file_path);
                            file_path = file_path.substr(0, pos).append(output_suffix);
                            OUTPUTS.push_back(file_path);
                     }
              }
              if (INPUTS.empty())
              {
                     return;
              }
       }
       else if (INPUTS.size() != OUTPUTS.size())
       {
              throw std::runtime_error("input count is different from output count");
       }
       for (size_t i = 0; i < INPUTS.size(); ++i)
       {
              std::cout << "Processing : " << INPUTS[i] << std::endl;
              ilzn::Picture picture;
              picture.load(INPUTS[i].c_str());
              for (auto &proc : PROCS)
              {
                     proc(picture);
              }
              picture.write(OUTPUTS[i].c_str(), OUTPUT_TYPE);
       }
}

int main(int argc, char *argv[])
{

       try
       {
              if (argc > 1)
              {
                     parse_args(argc - 1, argv + 1);
                     process();
              }
              else
              {
                     print_help();
              }
       }
       catch (const std::exception &e)
       {
              std::cerr << "[ERROR] " << e.what() << std::endl;
              std::cerr << "Aborting..." << std::endl;
       }
       return 0;
}
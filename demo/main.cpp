#include <Crawler.hpp>

int main(int argc, const char *argv[])
{
    try
    {
        po::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Help screen")
                ("url,u", po::value<string>(), "URL website")
                ("depth,d", po::value<int>()->default_value(0), "Depth find")
                ("network_threads,n", po::value<int>()->default_value(1),
                        "Number of threads for loading webpages")
                ("parser_threads,p", po::value<int>()->default_value(1),
                 "Number of threads for processing webpages")
                ("output,o", po::value<string>(), "Path to outout file");

        po::variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help"))
            std::cout << desc << '\n';
        else if (vm.count("url") && vm.count("depth") && vm.count("network_threads")
        && vm.count("parser_threads") && vm.count("output")){
          Crawl crawl(vm["depth"].as<int>(), vm["output"].as<string>());
          crawl.start(vm["url"].as<string>(), vm["network_threads"].as<int>(), vm["parser_threads"].as<int>());
        } else
            throw po::error("**********BAD SYNTAX**********\n"
                            "Look to --help or -h and write all options");
    } catch (const po::error &ex)
    {
        std::cerr << ex.what() << '\n';
    }
//  Crawl crawl(1, "../cmake-build-debug/out.txt");
//  crawl.start("http://wokandbox.ru/", 1, 1);
}
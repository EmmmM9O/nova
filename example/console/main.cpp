#include <core/Log.hpp>

int main() {
  nova::Log::my_logger.logDir = std::filesystem::current_path() / "logs";
  Log_info("example : console");
  Log_info(
      "color test: {color:$violet} violet \n"
      " {color:$106;90;205} 106,90,205 \n"
      " {color:$#8fb2c9} #8fb2c9");
}

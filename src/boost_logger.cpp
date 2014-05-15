#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/named_scope.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

// TODO: use name scopes instead
#define LOG_INFO() BOOST_LOG_TRIVIAL (info) << "[" << std::string (__FILE__) \
                                            << ":" << __LINE__ << "] "
int main()
{
  namespace logging  = boost::log;
  namespace src      = boost::log::sources;
  namespace expr     = boost::log::expressions;
  namespace keywords = boost::log::keywords;
  namespace trivial  = boost::log::trivial;
  namespace attrs    = boost::log::attributes;

  logging::add_file_log
    (keywords::file_name = "test.log",
     keywords::format = expr::format ("[%1%][%2%]%3%")
      % expr::format_date_time<boost::posix_time::ptime>
        ("TimeStamp", "%d/%m/%Y %T")
      % logging::trivial::severity
      % expr::smessage
     );

  logging::core::get ()->set_filter (trivial::severity >= trivial::info);
  logging::add_common_attributes ();

  LOG_INFO () << "This is a test." << std::endl;

  return 0;
}

#include "Instance.hpp"

#include "Logger.hpp"
#include "DefaultLogger.hpp"
#if !JALOG_NO_BUILTIN_ASYNC
#include "AsyncLogging.hpp"
#include "AsyncLoggingThread.hpp"
#endif

namespace jalog
{
Instance::Instance() : Instance(DefaultLogger()) {}
Instance::Instance(Logger& l) : m_logger(l) {}
Instance::~Instance() = default;

Instance::SetupDSL::~SetupDSL() {
    m_instance.m_logger.initialize();
}

Instance::SetupDSL& Instance::SetupDSL::defaultLevel(Level lvl) {
    m_instance.m_logger.setDefaultLevel(lvl);
    return *this;
}

Instance::SetupDSL& Instance::SetupDSL::add(SinkPtr sink) {
#if !JALOG_NO_BUILTIN_ASYNC
    if (m_instance.m_asyncLogging)
        m_instance.m_asyncLogging->add(sink);
    else
#endif
        m_instance.m_logger.addSink(sink);

    return *this;
}

Instance::SetupDSL& Instance::SetupDSL::async() {
#if !JALOG_NO_BUILTIN_ASYNC
    if (m_instance.m_asyncLogging) return *this;
    m_instance.m_asyncLogging = std::make_shared<AsyncLogging>();
    m_instance.m_logger.addSink(m_instance.m_asyncLogging);
    m_instance.m_asyncLoggingThread.reset(new AsyncLoggingThread(*m_instance.m_asyncLogging));
#endif
    return *this;
}

}

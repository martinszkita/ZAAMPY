#ifndef XMLINTERP4CONFIG_HH
#define XMLINTERP4CONFIG_HH

#include "Configuration.hh"
#include <xercesc/sax2/DefaultHandler.hpp>
#include <string>

class XMLInterp4Config : public xercesc::DefaultHandler {
public:
    XMLInterp4Config(Configuration &rConfig);
    XMLInterp4Config();

    virtual void startDocument() override;
    virtual void endDocument() override;
    virtual void startElement(const XMLCh *const pURI,
                              const XMLCh *const pLocalName,
                              const XMLCh *const pQName,
                              const xercesc::Attributes &rAttrs) override;
    virtual void endElement(const XMLCh *const pURI,
                            const XMLCh *const pLocalName,
                            const XMLCh *const pQName) override;

    void fatalError(const xercesc::SAXParseException &exception) override;

private:
    Configuration &_config;

    void ProcessLibAttrs(const xercesc::Attributes &rAttrs);
    void ProcessCubeAttrs(const xercesc::Attributes &rAttrs);
};

#endif

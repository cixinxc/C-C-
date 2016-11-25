#include <stdio.h>
#include <libxml/xmlreader.h>
#include <libxml/tree.h>
static void processNode(xmlTextReaderPtr reader) {
    const xmlChar *name, *value;
       
    name = xmlTextReaderConstName(reader);
    if (name == NULL)
        name = BAD_CAST "--";
   
    value = xmlTextReaderConstValue(reader);
    if (value != NULL && strcmp(value, "\n") != 0)
        printf("\"%s\"\n", value);
} 
streamFile(const char *filename) {
    xmlTextReaderPtr reader;
    int ret;
    reader = xmlReaderForFile(filename, "GB2312", 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while(ret == 1) {
            processNode(reader);
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            perror("parse");
        }
    } else {
        perror("open");
    }
}
int main(int argc, char **argv)
{
    if (argc < 2)
        return;
    streamFile(argv[1]);
    xmlCleanupParser();
    xmlMemoryDump();
    return 0;
}

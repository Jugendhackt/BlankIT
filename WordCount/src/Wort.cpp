#include "Wort.h"

Wort::Wort(string wo, unsigned int co, unsigned int id)
{
    //oftInSatz.set_empty_key(4294967294);
    oftInSatz.set_deleted_key(4294967295);
    //oftInDir.set_empty_key(4294967294);
    oftInDirV.set_deleted_key(4294967295);
    oftInDirH.set_deleted_key(4294967295);
    //oftInText.set_empty_key("%%%-leer-%%%");
    oftInText.set_deleted_key(4294967295);
    attr.set_deleted_key("%%%-gelöscht-%%%");
    flags.set_deleted_key("%%%-gelöscht-%%%");
    glW.set_deleted_key(4294967295);
    w = wo;
    c = co;
    WID = id;
}
Wort::Wort()
{
    //oftInSatz.set_empty_key(4294967294);
    oftInSatz.set_deleted_key(4294967295);
    //oftInDir.set_empty_key(4294967294);
    oftInDirV.set_deleted_key(4294967295);
    oftInDirH.set_deleted_key(4294967295);
    //oftInText.set_empty_key("%%%-leer-%%%");
    oftInText.set_deleted_key(4294967295);
    attr.set_deleted_key("%%%-gelöscht-%%%");
    flags.set_deleted_key("%%%-gelöscht-%%%");
    glW.set_deleted_key(4294967295);
    w = "";
    c = 0;
    WID=0;
}

Wort::~Wort()
{
    oftInSatz.clear();
    oftInDirV.clear();
    oftInDirH.clear();
    oftInText.clear();
    attr.clear();
    flags.clear();
    glW.clear();
}

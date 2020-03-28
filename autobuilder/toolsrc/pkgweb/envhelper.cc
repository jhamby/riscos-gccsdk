// Helper to give hints on what the environment field means

#include "envhelper.h"
#include <map>
#include <vector>



std::map<std::string, std::string> s_envCodeAndText =
{
    {"any", "This package should work with any system."},
    {"arm",   "This is 26/32 bit (addressing) neutral."
    " It may need the 32 bit version of the Shared C Library."
    " e.g. RiscPCs and later machines."
    },
    {"arm26", 
    "This package needs a machine that runs with 26 bit addressing."
    " e.g. RiscPCs and earlier."},
    {"arm32",   "This package requires RISC OS systems that can run 32 bit code."
    " e.g. Iyonix and later machines."
    },
    {"vfp", 
    "This application requires a machine with the Vector floating point unit."
    " e.g. Anything after the Iyonix that runs RISC OS 5."
    },
    {"swp",
     "This package requires the machine code SWP instruction that was removed with the ARMv8."
     " e.g. It won't work on the Raspberry Pi 3."
    }
};

std::map<std::string, std::string> s_modsAndText = 
{
    {"vchiq","The VCHIQ module fitted to the Raspberry Pi range of computers."}
};

static bool parse_field(std::vector<std::string> &items, const std::string &field)
{
    std::string item;
    bool skip = true;
    bool skip_char;

    for(std::string::const_iterator ci = field.begin(); ci !=field.end(); ++ci)
    {
        const char &ch = *ci;
        skip_char = (ch == ' ' || ch == ',');
        if (skip)
        {
            if (!skip_char)
            {
                item = ch;
                skip = false;
            }
        } else
        {
            if (skip_char)
            {
                if (!item.empty())
                {
                    items.push_back(item);
                    item.clear();
                    skip = true;
                }
            } else
            {
                item += ch;
            }
            
        }
    }
    if (!item.empty()) items.push_back(item);

    return !items.empty();
}

std::string get_environment_text(const pkg::control ctrl)
{
    std::string text;
    std::string env = ctrl.environment();
    std::string osdepends = ctrl.osdepends();
    if (!env.empty() || !osdepends.empty())
    {
        text = "<b>This package is designed to work on a system with the following environment</b><BR>";

        std::vector<std::string> envs;
        if (parse_field(envs, env))
        {
            text += "<ul>";
            std::string env_desc;
            for(std::vector<std::string>::const_iterator ei = envs.begin();
                ei != envs.end();++ei)
            {
                const std::string &e = *ei;
                std::map<std::string,std::string>::const_iterator found = s_envCodeAndText.find(e);
                text+="<li>";
                if (found != s_envCodeAndText.end())
                {
                    text += found->second;
                } else
                {
                    text += e + " environment code";
                }
                text += "</li>";                
            }
            text +="</ul>";
        }

        if (!osdepends.empty())
        {
            std::vector<std::string> mods;
            if (parse_field(mods, osdepends))
            {
                if (!text.empty()) text += "<br>";
                text += "The following modules must be installed and loaded at startup<br>";
                text += "<ul>";
                for (std::vector<std::string>::const_iterator osi = mods.begin();
                    osi != mods.end(); ++osi)
                {
                    const std::string &osd = *osi;
                    std::map<std::string,std::string>::const_iterator found = s_modsAndText.find(osd);
                    text += "<li>";
                    if (found != s_modsAndText.end())
                    {
                        text += found->second;
                    } else
                    {
                        text += osd;
                    }
                    text += "</li>";
                }
                text += "</ul>";
            }
        }
    }
    return text;
}

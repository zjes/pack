/* =========================================================================================================================================
    ____ __ _ ____ __  __
   |    |  ` |    |  /  /
   | |  | |  | |__|    /
   | ___| |  | |  |    \
   |_|  |__,_|____|__\__\ DSO library

   Copyright (C) 2020 Eaton
   Copyright (C) 2020-2022 zJes

   This program is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
========================================================================================================================================= */
#include "filegenerator.h"
#include "classgenerator.h"
#include "formatter.h"
#include "utils.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <iomanip>
#include <sstream>

namespace google::protobuf::compiler::pack {

static void Flatten(const Descriptor* descriptor, std::vector<const Descriptor*>* flatten)
{
    for (int i = 0; i < descriptor->nested_type_count(); i++) {
        Flatten(descriptor->nested_type(i), flatten);
    }
    flatten->push_back(descriptor);
}

static std::vector<const Descriptor*> FlattenMessagesInFile(const FileDescriptor* file)
{
    std::vector<const Descriptor*> result;
    for (int i = 0; i < file->message_type_count(); i++) {
        Flatten(file->message_type(i), &result);
    }
    return result;
}

FileGenerator::FileGenerator(const FileDescriptor* file)
    : m_file(file)
{
    std::vector<const Descriptor*> msgs = FlattenMessagesInFile(m_file);
    for (const auto& msg : msgs) {
        m_generators.emplace_back(msg);
    }
}

void FileGenerator::generateHeader(io::Printer& printer) const
{
    Formatter frm(printer);

    frm << "#pragma once\n";

    for (int i = 0; i < m_file->dependency_count(); ++i) {
        auto dep = m_file->dependency(i);
        frm << "#include \"" << genFileName(dep) << "\"\n";
    }
    frm << "#include <pack/pack.h>\n";
    frm << "\n";

    std::string path  = "file";
    size_t      index = m_file->name().find_last_of('.');
    if (index != std::string::npos) {
        path                     = m_file->name().substr(0, index);
        std::string::size_type n = 0;
        while ((n = path.find("/", n)) != std::string::npos) {
            path.replace(n, 1, "::");
            ++n;
        }
        n = 0;
        while ((n = path.find("-", n)) != std::string::npos) {
            path.replace(n, 1, "_");
            ++n;
        }
    }

    std::stringstream ss;
    bool              wasText = false;
    bool              wasHex  = false;
    for (const char& ch : getDescriptor()) {
        if (std::isalnum(ch)) {
            ss << (wasHex ? "\" \"" : "") << ch;
            wasText = true;
            wasHex  = false;
        } else {
            ss << (wasText ? "\" \"" : "") << "\\x" << std::setfill('0') << std::setw(2) << std::hex
               << static_cast<uint32_t>(static_cast<uint8_t>(ch));
            wasHex  = true;
            wasText = false;
        }
    }
    frm << "// "
           "================================================================================================="
           "==========\n";
    frm << "// Protobuf file descriptor\n";
    frm << "// "
           "================================================================================================="
           "==========\n";
    frm << "namespace " << path << " {\n\n";
    frm << "inline const std::string& descriptor()\n";
    frm << "{\n";
    frm.indent();
    frm << "static std::string desc(\"" << ss.str() << "\", " << ss.str().size() << ");\n";
    frm << "return desc;\n";
    frm.outdent();
    frm << "}\n";
    frm << "}\n\n";

    frm << "// "
           "================================================================================================="
           "==========\n";
    frm << "// Pack files\n";
    frm << "// "
           "================================================================================================="
           "==========\n";
    if (!m_file->package().empty()) {
        frm << "namespace " << m_file->package() << " {\n\n";
    }

    for (int i = 0; i < m_file->message_type_count(); i++) {
        ClassGenerator gen(m_file->message_type(i));
        gen.generateHeader(frm, path);
    }

    // Generate enums conversions
    for (int i = 0; i < m_file->message_type_count(); i++) {
        frm << "\n";
        auto desc = m_file->message_type(i);
        for (int e = 0; e < desc->enum_type_count(); ++e) {
            const auto& type = desc->enum_type(e);

            std::string            name = type->full_name();
            std::string::size_type n    = 0;
            while ((n = name.find(".", n)) != std::string::npos) {
                name.replace(n, 1, "::");
                ++n;
            }

            frm << "inline std::ostream& operator<<(std::ostream& ss, " << name << " value)\n";
            frm << "{\n";
            frm.indent();
            frm << "switch(value){\n";
            frm.indent();
            for (int j = 0; j < type->value_count(); ++j) {
                const auto& val = type->value(j);
                frm << "case " << name << "::" << val->name() << ":\n";
                frm.indent();
                frm << "ss << \"" << val->name() << "\";\n";
                frm << "break;\n";
                frm.outdent();
            }
            frm.outdent();
            frm << "}\n";
            frm << "return ss;\n";
            frm.outdent();
            frm << "}\n";
            frm << "\n";
            frm << "inline std::istream& operator>>(std::istream& ss, " << name << "& value)\n";
            frm << "{\n";
            frm.indent();
            frm << "std::string strval;\n";
            frm << "ss >> strval;\n";
            for (int j = 0; j < type->value_count(); ++j) {
                const auto& val = type->value(j);
                frm << (j != 0 ? "} else " : "") << "if (strval == \"" << val->name() << "\") {\n";
                frm.indent();
                frm << "value = " << name << "::" << val->name() << ";\n";
                frm.outdent();
                if (j == type->value_count() - 1) {
                    frm << "}\n";
                }
            }
            frm << "return ss;\n";
            frm.outdent();
            frm << "}\n";
        }
    }

    if (!m_file->package().empty()) {
        frm << "}\n";
    }
    frm.outdent();
}

static void grabDependencies(std::set<const FileDescriptor*>& toWrite, const FileDescriptor* desc)
{
    for (int i = 0; i < desc->dependency_count(); i++) {
        const FileDescriptor* dependency = desc->dependency(i);
        toWrite.insert(dependency);
        grabDependencies(toWrite, dependency);
    }
}

std::string FileGenerator::getDescriptor() const
{
    FileDescriptorSet               fileSet;
    std::set<const FileDescriptor*> toWrite;
    toWrite.insert(m_file);
    grabDependencies(toWrite, m_file);

    auto file = fileSet.mutable_file();
    for (const auto& desc : toWrite) {
        auto newDep = file->Add();
        desc->CopyTo(newDep);
    }

    return fileSet.SerializeAsString();
}

FileGenerator::~FileGenerator()
{
}

} // namespace google::protobuf::compiler::pack

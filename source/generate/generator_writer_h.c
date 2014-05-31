#include "generate/generator_writer_h.h"
#include "generate/generator_types_h.h"
#include "symbols.h"
#include "generator.h"
#include "version.h"

#include <stdio.h>
#include <string.h>

static error_code_t on_document_begin(generator_t *super, const YYLTYPE *yylloc, const char *file_name)
{
	char types_header[MAX_PACKAGE_NAME_LENGTH];	
	TLIBC_UNUSED(yylloc);

	generator_open(super, file_name, GENERATOR_WRITER_H_SUFFIX);

	generator_printline(super, 0, "/**");
    generator_printline(super, 0, " * Autogenerated by %s Compiler (%s)", PROJECT_NAME, VERSION);
    generator_printline(super, 0, " *");
    generator_printline(super, 0, " * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING");
    generator_printline(super, 0, " *  @generated");
    generator_printline(super, 0, " */");
	generator_printline(super, 0, "");



	generator_printline(super, 0, "#ifndef _H_%s", super->document_name);
	generator_printline(super, 0, "#define _H_%s", super->document_name);
	generator_printline(super, 0, "");
	generator_printline(super, 0, "#include <stdint.h>");
	generator_printline(super, 0, "#include \"protocol/tlibc_abstract_writer.h\"");
	generator_printline(super, 0, "#include \"core/tlibc_error_code.h\"");
	
	//包含types的头文件
	strncpy_notdir(types_header, file_name, MAX_PACKAGE_NAME_LENGTH - 1);
	generator_replace_extension(types_header, MAX_PACKAGE_NAME_LENGTH, GENERATOR_TYPES_H_SUFFIX);
	generator_printline(super, 0, "#include \"%s\"", types_header);
	
	generator_printline(super, 0, "");
	generator_printline(super, 0, "");
	return E_TD_NOERROR;
}

static error_code_t on_document_end(generator_t *super, const YYLTYPE *yylloc, const char *file_name)
{
	TLIBC_UNUSED(file_name);
	TLIBC_UNUSED(yylloc);

	generator_printline(super, 0, "");
	generator_printline(super, 0, "#endif //_H_%s", super->document_name);
	generator_printline(super, 0, "");

	generator_close(super);
	return E_TD_NOERROR;
}

static error_code_t _on_import(td_generator_writer_h_t *self, const syn_import_t *de_import)
{
	char name[MAX_PACKAGE_NAME_LENGTH];	
	strncpy_notdir(name, de_import->package_name, MAX_PACKAGE_NAME_LENGTH - 1);
	generator_replace_extension(name, MAX_PACKAGE_NAME_LENGTH, GENERATOR_WRITER_H_SUFFIX);
	generator_printline(&self->super, 0, "#include \"%s\"", name);
	generator_printline(&self->super, 0, "");

	return E_TD_NOERROR;
}



static error_code_t _on_enum(td_generator_writer_h_t *self, const syn_enum_t *de_enum)
{
	generator_printline(&self->super, 0, "tlibc_error_code_t tlibc_write_%s(tlibc_abstract_writer_t *self, const enum %s *data);", de_enum->name, de_enum->name);
	generator_printline(&self->super, 0, "");

	return E_TD_NOERROR;
}

static error_code_t _on_struct(td_generator_writer_h_t *self, const syn_struct_t *de_struct)
{
	generator_printline(&self->super, 0, "tlibc_error_code_t tlibc_write_%s(tlibc_abstract_writer_t *self, const struct %s *data);", de_struct->name, de_struct->name);
	generator_printline(&self->super, 0, "");

	return E_TD_NOERROR;
}

static error_code_t _on_union(td_generator_writer_h_t *self, const syn_union_t *de_union)
{
	generator_print(&self->super, 0, "tlibc_error_code_t tlibc_write_%s(tlibc_abstract_writer_t *self, const union %s *data, ", de_union->name, de_union->name);
	generator_print_ctype(&self->super, &de_union->parameters.par_list[0].type);
	generator_printline(&self->super, 0, " selector);");
	generator_printline(&self->super, 0, "");
	return E_TD_NOERROR;
}

static error_code_t on_definition(generator_t *super, const YYLTYPE *yylloc, const syn_definition_t *definition)
{
	td_generator_writer_h_t *self = TLIBC_CONTAINER_OF(super, td_generator_writer_h_t, super);
	TLIBC_UNUSED(yylloc);
	switch(definition->type)
	{
		case E_DT_IMPORT:
			return _on_import(self, &definition->definition.de_import);				
		case E_DT_CONST:
			return E_TD_NOERROR;
		case E_DT_ENUM:
			return _on_enum(self, &definition->definition.de_enum);
		case E_DT_STRUCT:
			return _on_struct(self, &definition->definition.de_struct);
		case E_DT_UNION:
			return _on_union(self, &definition->definition.de_union);
		case E_DT_TYPEDEF:
			return E_TD_NOERROR;
		case E_DT_UNIX_COMMENT:
			return E_TD_NOERROR;
		default:
			return E_TD_ERROR;
	}
}

void generator_writer_h_init(td_generator_writer_h_t *self, const symbols_t *symbols)
{
	generator_init(&self->super, symbols);

	self->super.on_document_begin = on_document_begin;
	self->super.on_document_end = on_document_end;
	self->super.on_definition = on_definition;
}

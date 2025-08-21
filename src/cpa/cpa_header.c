#include "cpa_utils.h"

void cpassembly_emit_header(CPAssembly *self)
{
    IOStream *io = &self->output;

    fprintf(io->stream, "\t.file\t\"%s\"\n", self->src);
    fprintf(io->stream, "\t.text\n");
    fprintf(io->stream, "\t.globl\n");
    fprintf(io->stream, "\t.type\n");
}

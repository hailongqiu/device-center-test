#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#define get_str(field_name, ptr)              \
    if (g_str_has_prefix(tmp[0], field_name)) \
    {                                         \
        ptr = g_strdup(tmp[1]);               \
        g_strfreev(tmp);                      \
        continue;                             \
    }

#define get_int(field_name, ptr)              \
    if (g_str_has_prefix(tmp[0], field_name)) \
    {                                         \
        ptr = atoi(tmp[1]);                   \
        g_strfreev(tmp);                      \
        continue;                             \
    }

#define get_float(field_name, ptr)            \
    if (g_str_has_prefix(tmp[0], field_name)) \
    {                                         \
        ptr = atof(tmp[1]);                   \
        g_strfreev(tmp);                      \
        continue;                             \
    }

typedef struct _Processor Processor;

struct _Processor
{
    gchar *model_name;
    gchar *vendor_id;
    gchar *flags;
    gint cache_size;
    gfloat bogomips;
    gfloat cpu_mhz;

    gchar * has_fpu;
    gchar *bug_fdiv;
    gchar *bug_hlt;
    gchar *bug_f00f;
    gchar *bug_coma;

    gint model;
    gint family;
    gint stepping;
    
    gchar *strmodel;

    gint id;

    GList *cache;
};

int main(int argc, char *argv[])
{
    
    GSList *proces = NULL;
    Processor *processor = NULL;
    FILE *cpuinfo;
    gchar buffer[256];
    gint processor_number = 0;
    gchar *str1;

    cpuinfo = fopen("/proc/cpuinfo", "r");
    if (!cpuinfo)
    {
        return FALSE;
    }
    //
    while (fgets(buffer, 256, cpuinfo))
    {
        gchar **tmp = g_strsplit(buffer, ":", 2);

        if (g_str_has_prefix(tmp[0], "processor"))
        {
            if (processor)
            {
                gint family = processor->family;
                gint model = processor->model;
                processor->strmodel = g_strdup("Pentium II/Plel");
                proces = g_slist_append(proces, processor);
            }
            processor = g_new0(Processor, 1);
            processor_number++;
        }

        if (tmp[0] && tmp[1])
        {
            // 去除前面和后面的空格.
            tmp[0] = g_strstrip(tmp[0]);
            tmp[1] = g_strstrip(tmp[1]);
            // 获取cpu的信息.
            get_str("model name",   processor->model_name);
            get_str("vendor_id",    processor->vendor_id);
            get_str("flags",        processor->flags);
            get_int("cache size",   processor->cache_size);
            get_float("cpu MHz",    processor->cpu_mhz);
            get_float("bogomips",   processor->bogomips);
            get_str("fpu", processor->has_fpu);
        }
        g_strfreev(tmp);
    }

    if (processor)
    {
        proces = g_slist_append(proces, processor);
    }
    //
    fclose(cpuinfo);
    printf("cache_size:%d\n", ((Processor*)proces->data)->cache_size);
    printf("model name:%s\n", ((Processor*)proces->data)->model_name);
    printf("bogomips:%.2f\n", ((Processor*)proces->data)->bogomips);
    printf("bogomips:%.2f\n", ((Processor*)proces->next->data)->bogomips);
    printf("cpu个数:%d\n", processor_number);

    return TRUE;
}




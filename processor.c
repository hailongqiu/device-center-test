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

    gchar *fdiv_bug;
    gchar *hlt_bug;
    gchar *f00f_bug;
    gchar *coma_bug;

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
            //
            get_str("fpu", processor->has_fpu);
            //
            get_str("fdiv_bug", processor->fdiv_bug);
            get_str("hlt_bug",  processor->hlt_bug);
            get_str("f00f_bug", processor->f00f_bug);
            get_str("coma_bug", processor->coma_bug);
            //
            get_int("processor", processor->id);
        }
        g_strfreev(tmp);
    }

    if (processor)
    {
        proces = g_slist_append(proces, processor);
    }
    //
    fclose(cpuinfo);
    puts("================输出cpu信息========================");
    printf("cpu个数:%d\n", processor_number);
    GSList *temp_proces = NULL;
    for (temp_proces = proces; temp_proces != NULL ; temp_proces = temp_proces->next)
    {
        puts("---------------------------------------------------");
        printf("id:%d\n", ((Processor*)temp_proces->data)->id);
        printf("model name:%s\n", ((Processor*)temp_proces->data)->model_name);
        printf("cache_size:%d\n", ((Processor*)temp_proces->data)->cache_size);
        printf("bogomips:%.2f\n", ((Processor*)temp_proces->data)->bogomips);
    }
    return TRUE;
}




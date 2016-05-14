#include <stdio.h>
#include <gettext-po.h>
#include <string.h>
#include <time.h>

void error_fn (int a, po_message_t b, const char *c, size_t d, size_t e,
    int f, const char *g) {}

void error2_fn (int a, po_message_t b, const char *c, size_t d, size_t e,
    int f, const char *g, po_message_t h, const char *i, size_t j, size_t k,
    int l, const char *m) {}

typedef struct {
  int repository_id;
  char filepath[4096];
  char now[1024];
} file_t;

int callback (const char* msgid, const char* msgstr, void* a_user_data)
{
  const file_t* f = a_user_data;
  printf("(%d, '%s', '%s', '%s', '%s', '%s');\n", f->repository_id, f->filepath, msgid, msgstr, f->now, f->now);
}

int po_iterate (const char* filename, void* a_user_data)
{
  struct po_xerror_handler derp;

  po_file_t             fp = NULL;
  po_message_iterator_t it = NULL;
  po_message_t          message = NULL;
  const char* const*    domains = NULL;
  const char* const*    domain = NULL;

  derp.xerror = &error_fn;
  derp.xerror2 = &error2_fn;

  if ((fp = po_file_read(filename, &derp)) == NULL) {
    puts("error opening po");
    return -1;
  }

  domains = po_file_domains(fp);
  for (domain = domains; *domain; ++domain) {
    it = po_message_iterator(fp, *domain);

    while ((message = po_next_message(it)) != NULL) {
      const char* msgid = NULL;
      const char* msgstr = NULL;
      callback(po_message_msgid(message), po_message_msgstr(message), a_user_data);

      msgid = po_message_msgid_plural(message);
      for (int i = 0; (msgstr = po_message_msgstr_plural(message, i)); ++i) {
        callback(msgid, msgstr, a_user_data);
      }
    }
  }

  po_file_free(fp);
}

int main (int argc, char* argv[])
{
  file_t f;

  time_t now_raw;
  struct tm* now_timeinfo;

  time(&now_raw);
  now_timeinfo = localtime(&now_raw);
  strftime(f.now, 1024, "%Y-%m-%d %H:%M:%S.000000", now_timeinfo);
  f.repository_id = atoi(argv[1]);
  strncpy(f.filepath, argv[2], 4096);

  puts("BEGIN;");
  puts("INSERT INTO translations (repository_id, filepath, msgid, msgstr, created_at, updated_at) VALUES");
  po_iterate(f.filepath, &f);
  puts("COMMIT;");
  return 0;
}

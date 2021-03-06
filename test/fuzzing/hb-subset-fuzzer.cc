#include "hb-fuzzer.hh"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hb-subset.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
  hb_blob_t *blob = hb_blob_create ((const char *)data, size,
                                    HB_MEMORY_MODE_READONLY, NULL, NULL);
  hb_face_t *face = hb_face_create (blob, 0);
  hb_subset_profile_t *profile = hb_subset_profile_create ();

  hb_subset_input_t *input = hb_subset_input_create_or_fail ();
  *hb_subset_input_drop_hints(input) = false;
  hb_set_t *codepoints = hb_subset_input_unicode_set (input);

  hb_subset_input_t *input_drop_hints = hb_subset_input_create_or_fail ();
  *hb_subset_input_drop_hints(input_drop_hints) = true;
  hb_set_t *codepoints_drop_hints = hb_subset_input_unicode_set (input);

  const hb_codepoint_t text[] =
      {
        'A', 'B', 'C', 'D', 'E', 'X', 'Y', 'Z', '1', '2',
        '3', '@', '_', '%', '&', ')', '*', '$', '!'
      };
  for (int i = 0; i < sizeof (text) / sizeof (hb_codepoint_t); i++)
  {
    hb_set_add (codepoints, text[i]);
    hb_set_add (codepoints_drop_hints, text[i]);
  }

  hb_face_t *result = hb_subset (face, profile, input);
  hb_face_destroy (result);

  result = hb_subset (face, profile, input_drop_hints);
  hb_face_destroy (result);

  hb_subset_input_destroy (input);
  hb_subset_input_destroy (input_drop_hints);
  hb_subset_profile_destroy (profile);
  hb_face_destroy (face);
  hb_blob_destroy (blob);

  return 0;
}

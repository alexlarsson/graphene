/* graphene-point.c: Point
 *
 * Copyright © 2014  Emmanuele Bassi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * SECTION:graphene-point
 * @Title: Point
 * @Short_Description: A point with 2 coordinates
 *
 * #graphene_point_t is a data structure capable of describing a point with
 * two coordinates:
 *
 *  * @graphene_point_t.x
 *  * @graphene_point_t.y
 */

#include "config.h"

#include <math.h>

#include "graphene-point.h"
#include "graphene-simd4f.h"

/**
 * graphene_point_alloc: (constructor)
 *
 * Allocates a new #graphene_point_t structure.
 *
 * Returns: (transfer full): the newly allocated #graphene_point_t.
 *   Use graphene_point_free() to free the resources allocated by
 *   this function.
 *
 * Since: 1.0
 */
graphene_point_t *
graphene_point_alloc (void)
{
  return calloc (1, sizeof (graphene_point_t));
}

/**
 * graphene_point_free:
 *
 * Frees the resources allocated by graphene_point_alloc().
 *
 * Since: 1.0
 */
void
graphene_point_free (graphene_point_t *p)
{
  free (p);
}

graphene_point_t *
graphene_point_init (graphene_point_t *p,
                     float             x,
                     float             y)
{
  graphene_return_val_if_fail (p != NULL, NULL);

  p->x = x;
  p->y = y;

  return p;
}

graphene_point_t *
graphene_point_init_from_point (graphene_point_t       *p,
                                const graphene_point_t *src)
{
  graphene_return_val_if_fail (p != NULL, NULL);
  graphene_return_val_if_fail (src != NULL, p);

  *p = *src;

  return p;
}

bool
graphene_point_equal (const graphene_point_t *a,
                      const graphene_point_t *b)
{
  if (a == b)
    return true;

  if (a == NULL || b == NULL)
    return false;

  return graphene_point_near (a, b, GRAPHENE_FLOAT_EPSILON);
}

float
graphene_point_distance (const graphene_point_t *a,
                         const graphene_point_t *b,
                         float                  *d_x,
                         float                  *d_y)
{
  graphene_simd4f_t s_a, s_b, res;

  graphene_return_val_if_fail (a != NULL && b!= NULL, 0.f);

  if (a == b)
    return 0.f;

  s_a = graphene_simd4f_init (a->x, a->y, 0.f, 0.f);
  s_b = graphene_simd4f_init (b->x, b->y, 0.f, 0.f);
  res = graphene_simd4f_sub (s_a, s_b);

  if (d_x != NULL)
    *d_x = fabsf (graphene_simd4f_get_x (res));

  if (d_y != NULL)
    *d_y = fabsf (graphene_simd4f_get_y (res));

  return graphene_simd4f_get_x (graphene_simd4f_length2 (res));
}

bool
graphene_point_near (const graphene_point_t *a,
                     const graphene_point_t *b,
                     float                   epsilon)
{
  graphene_simd4f_t s_a, s_b, res;

  graphene_return_val_if_fail (a != NULL && b != NULL, false);

  if (a == b)
    return true;

  s_a = graphene_simd4f_init (a->x, a->y, 0.f, 0.f);
  s_b = graphene_simd4f_init (b->x, b->y, 0.f, 0.f);
  res = graphene_simd4f_sub (s_a, s_b);

  return fabsf (graphene_simd4f_get_x (res)) < epsilon &&
         fabsf (graphene_simd4f_get_y (res)) < epsilon;
}

void
graphene_point_interpolate (const graphene_point_t *a,
                            const graphene_point_t *b,
                            double                  factor,
                            graphene_point_t       *res)
{
  graphene_return_if_fail (a != NULL && b != NULL);
  graphene_return_if_fail (res != NULL);

  res->x = a->x + (b->x - a->x) * factor;
  res->y = a->y + (b->y - a->y) * factor;
}

static const graphene_point_t _graphene_point_zero = GRAPHENE_POINT_INIT_ZERO;

const graphene_point_t *
graphene_point_zero (void)
{
#ifdef GRAPHENE_ENABLE_DEBUG
  /* overzealous checks */
  graphene_assert (_graphene_point_zero.x == 0.f);
  graphene_assert (_graphene_point_zero.y == 0.f);
#endif

  return &_graphene_point_zero;
}

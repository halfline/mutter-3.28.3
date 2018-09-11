/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/*
 * Copyright (C) 2016 Red Hat
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef META_MONITOR_H
#define META_MONITOR_H

#include <glib-object.h>

#include "backends/meta-crtc.h"
#include "backends/meta-monitor-manager-private.h"
#include "backends/meta-output.h"

typedef struct _MetaMonitorSpec
{
  char *connector;
  char *vendor;
  char *product;
  char *serial;
} MetaMonitorSpec;

typedef struct _MetaMonitorModeSpec
{
  int width;
  int height;
  float refresh_rate;
  MetaCrtcModeFlag flags;
} MetaMonitorModeSpec;

typedef struct _MetaMonitorCrtcMode
{
  MetaOutput *output;
  MetaCrtcMode *crtc_mode;
} MetaMonitorCrtcMode;

#define HANDLED_CRTC_MODE_FLAGS (META_CRTC_MODE_FLAG_INTERLACE)

typedef gboolean (* MetaMonitorModeFunc) (MetaMonitor         *monitor,
                                          MetaMonitorMode     *mode,
                                          MetaMonitorCrtcMode *monitor_crtc_mode,
                                          gpointer             user_data,
                                          GError             **error);

typedef enum _MetaMonitorScalesConstraint
{
  META_MONITOR_SCALES_CONSTRAINT_NONE = 0,
  META_MONITOR_SCALES_CONSTRAINT_NO_FRAC = (1 << 0),
} MetaMonitorScalesConstraint;

#define META_TYPE_MONITOR (meta_monitor_get_type ())
G_DECLARE_DERIVABLE_TYPE (MetaMonitor, meta_monitor, META, MONITOR, GObject)

struct _MetaMonitorClass
{
  GObjectClass parent_class;

  MetaOutput * (* get_main_output) (MetaMonitor *monitor);
  void (* derive_layout) (MetaMonitor   *monitor,
                          MetaRectangle *layout);
  void (* calculate_crtc_pos) (MetaMonitor         *monitor,
                               MetaMonitorMode     *monitor_mode,
                               MetaOutput          *output,
                               MetaMonitorTransform crtc_transform,
                               int                 *out_x,
                               int                 *out_y);
  gboolean (* get_suggested_position) (MetaMonitor *monitor,
                                       int         *width,
                                       int         *height);
};

#define META_TYPE_MONITOR_NORMAL (meta_monitor_normal_get_type ())
G_DECLARE_FINAL_TYPE (MetaMonitorNormal, meta_monitor_normal,
                      META, MONITOR_NORMAL,
                      MetaMonitor)

#define META_TYPE_MONITOR_TILED (meta_monitor_tiled_get_type ())
G_DECLARE_FINAL_TYPE (MetaMonitorTiled, meta_monitor_tiled,
                      META, MONITOR_TILED,
                      MetaMonitor)

MetaMonitorTiled * meta_monitor_tiled_new (MetaGpu    *gpu,
                                           MetaOutput *output);

MetaMonitorNormal * meta_monitor_normal_new (MetaGpu    *gpu,
                                             MetaOutput *output);

MetaMonitorSpec * meta_monitor_get_spec (MetaMonitor *monitor);

MetaGpu * meta_monitor_get_gpu (MetaMonitor *monitor);

gboolean meta_monitor_is_active (MetaMonitor *monitor);

MetaOutput * meta_monitor_get_main_output (MetaMonitor *monitor);

gboolean meta_monitor_is_primary (MetaMonitor *monitor);

gboolean meta_monitor_supports_underscanning (MetaMonitor *monitor);

gboolean meta_monitor_is_underscanning (MetaMonitor *monitor);

gboolean meta_monitor_is_laptop_panel (MetaMonitor *monitor);

gboolean meta_monitor_is_same_as (MetaMonitor *monitor,
                                  MetaMonitor *other_monitor);

GList * meta_monitor_get_outputs (MetaMonitor *monitor);

void meta_monitor_get_current_resolution (MetaMonitor *monitor,
                                          int           *width,
                                          int           *height);

void meta_monitor_derive_layout (MetaMonitor   *monitor,
                                 MetaRectangle *layout);

void meta_monitor_get_physical_dimensions (MetaMonitor *monitor,
                                           int         *width_mm,
                                           int         *height_mm);

CoglSubpixelOrder meta_monitor_get_subpixel_order (MetaMonitor *monitor);

const char * meta_monitor_get_connector (MetaMonitor *monitor);

const char * meta_monitor_get_vendor (MetaMonitor *monitor);

const char * meta_monitor_get_product (MetaMonitor *monitor);

const char * meta_monitor_get_serial (MetaMonitor *monitor);

MetaConnectorType meta_monitor_get_connector_type (MetaMonitor *monitor);

/* This function returns the transform corrected for the panel orientation */
MetaMonitorTransform meta_monitor_logical_to_crtc_transform (MetaMonitor          *monitor,
                                                             MetaMonitorTransform  transform);
/*
 * This function converts a transform corrected for the panel orientation
 * to its logical (user-visible) transform.
 */
MetaMonitorTransform meta_monitor_crtc_to_logical_transform (MetaMonitor          *monitor,
                                                             MetaMonitorTransform  transform);

uint32_t meta_monitor_tiled_get_tile_group_id (MetaMonitorTiled *monitor_tiled);

gboolean meta_monitor_get_suggested_position (MetaMonitor *monitor,
                                              int         *x,
                                              int         *y);

MetaLogicalMonitor * meta_monitor_get_logical_monitor (MetaMonitor *monitor);

MetaMonitorMode * meta_monitor_get_mode_from_id (MetaMonitor *monitor,
                                                 const char  *monitor_mode_id);

MetaMonitorMode * meta_monitor_get_mode_from_spec (MetaMonitor         *monitor,
                                                   MetaMonitorModeSpec *monitor_mode_spec);

MetaMonitorMode * meta_monitor_get_preferred_mode (MetaMonitor *monitor);

MetaMonitorMode * meta_monitor_get_current_mode (MetaMonitor *monitor);

void meta_monitor_derive_current_mode (MetaMonitor *monitor);

void meta_monitor_set_current_mode (MetaMonitor     *monitor,
                                    MetaMonitorMode *mode);

GList * meta_monitor_get_modes (MetaMonitor *monitor);

void meta_monitor_calculate_crtc_pos (MetaMonitor         *monitor,
                                      MetaMonitorMode     *monitor_mode,
                                      MetaOutput          *output,
                                      MetaMonitorTransform crtc_transform,
                                      int                 *out_x,
                                      int                 *out_y);

float meta_monitor_calculate_mode_scale (MetaMonitor     *monitor,
                                         MetaMonitorMode *monitor_mode);

float * meta_monitor_calculate_supported_scales (MetaMonitor                *monitor,
                                                 MetaMonitorMode            *monitor_mode,
                                                 MetaMonitorScalesConstraint constraints,
                                                 int                        *n_supported_scales);

const char * meta_monitor_mode_get_id (MetaMonitorMode *monitor_mode);

MetaMonitorModeSpec * meta_monitor_mode_get_spec (MetaMonitorMode *monitor_mode);

void meta_monitor_mode_get_resolution (MetaMonitorMode *monitor_mode,
                                       int             *width,
                                       int             *height);

float meta_monitor_mode_get_refresh_rate (MetaMonitorMode *monitor_mode);

MetaCrtcModeFlag meta_monitor_mode_get_flags (MetaMonitorMode *monitor_mode);

gboolean meta_monitor_mode_foreach_crtc (MetaMonitor        *monitor,
                                         MetaMonitorMode    *mode,
                                         MetaMonitorModeFunc func,
                                         gpointer            user_data,
                                         GError            **error);

gboolean meta_monitor_mode_foreach_output (MetaMonitor        *monitor,
                                           MetaMonitorMode    *mode,
                                           MetaMonitorModeFunc func,
                                           gpointer            user_data,
                                           GError            **error);

MetaMonitorSpec * meta_monitor_spec_clone (MetaMonitorSpec *monitor_id);

gboolean meta_monitor_spec_equals (MetaMonitorSpec *monitor_id,
                                   MetaMonitorSpec *other_monitor_id);

int meta_monitor_spec_compare (MetaMonitorSpec *monitor_spec_a,
                               MetaMonitorSpec *monitor_spec_b);

void meta_monitor_spec_free (MetaMonitorSpec *monitor_id);

#endif /* META_MONITOR_H */

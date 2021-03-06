/*
 * libiax: An implementation of the Inter-Asterisk eXchange protocol
 *
 * Asterisk internal frame definitions.
 * 
 * Copyright (C) 1999, Mark Spencer
 *
 * Mark Spencer <markster@linux-support.net>
 *
 * This program is free software, distributed under the terms of
 * the GNU Lesser General Public License.  Other components of
 * Asterisk are distributed under The GNU General Public License
 * only.
 */

#ifndef _LIBIAX_FRAME_H
#define _LIBIAX_FRAME_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* Frame types */
#define AST_FRAME_DTMF          1         /* A DTMF digit, subclass is the digit */
#define AST_FRAME_VOICE         2         /* Voice data, subclass is AST_FORMAT_* */
#define AST_FRAME_VIDEO         3         /* Video frame, maybe?? :) */
#define AST_FRAME_CONTROL       4         /* A control frame, subclass is AST_CONTROL_* */
#define AST_FRAME_NULL          5         /* An empty, useless frame */
#define AST_FRAME_IAX           6         /* Inter Aterisk Exchange private frame type */
#define AST_FRAME_TEXT          7         /* Text messages */
#define AST_FRAME_IMAGE         8         /* Image Frames */
#define AST_FRAME_HTML          9         /* HTML Frames */
#define AST_FRAME_CNG           10        /* Comfort Noise frame (subclass is level of CNG in -dBov) */

/* HTML subclasses */
#define AST_HTML_URL            1         /* Sending a URL */
#define AST_HTML_DATA           2         /* Data frame */
#define AST_HTML_BEGIN          4         /* Beginning frame */
#define AST_HTML_END            8         /* End frame */
#define AST_HTML_LDCOMPLETE     16        /* Load is complete */
#define AST_HTML_NOSUPPORT      17        /* Peer is unable to support HTML */
#define AST_HTML_LINKURL        18        /* Send URL and track */
#define AST_HTML_UNLINK         19        /* Request no more linkage */
#define AST_HTML_LINKREJECT     20        /* Reject LINKURL */

/* Data formats for capabilities and frames alike */
#define AST_FORMAT_G723_1       (1 << 0)  /*! G.723.1 compression */
#define AST_FORMAT_GSM          (1 << 1)  /*! GSM compression */
#define AST_FORMAT_ULAW         (1 << 2)  /*! Raw mu-law data (G.711) */
#define AST_FORMAT_ALAW         (1 << 3)  /*! Raw A-law data (G.711) */
#define AST_FORMAT_G726         (1 << 4)  /*! ADPCM (G.726, 32kbps) */
#define AST_FORMAT_ADPCM        (1 << 5)  /*! ADPCM (IMA) */
#define AST_FORMAT_SLINEAR      (1 << 6)  /*! Raw 16-bit Signed Linear (8000 Hz) PCM */
#define AST_FORMAT_LPC10        (1 << 7)  /*! LPC10, 180 samples/frame */
#define AST_FORMAT_G729A        (1 << 8)  /*! G.729A audio */
#define AST_FORMAT_SPEEX        (1 << 9)  /*! SpeeX Free Compression */
#define AST_FORMAT_ILBC         (1 << 10) /*! iLBC Free Compression */
#define AST_FORMAT_MAX_AUDIO    (1 << 15) /*! Maximum audio format */
#define AST_FORMAT_JPEG         (1 << 16) /*! JPEG Images */
#define AST_FORMAT_PNG          (1 << 17) /*! PNG Images */
#define AST_FORMAT_H261         (1 << 18) /*! H.261 Video */
#define AST_FORMAT_H263         (1 << 19) /*! H.263 Video */
#define AST_FORMAT_H263p        (1 << 20) /*! H.263+ Video  */
#define AST_FORMAT_H264         (1 << 21) /*! H.264 Video*/
#define AST_FORMAT_MPEG4        (1 << 22) /*! MPEG4 Video*/
#define AST_FORMAT_THEORA       (1 << 24) /*! Theora Video */
#define AST_FORMAT_MAX_VIDEO    (1 << 24) /*! Maximum video format */

/* Control frame types */
#define AST_CONTROL_HANGUP      1         /* Other end has hungup */
#define AST_CONTROL_RING        2         /* Local ring */
#define AST_CONTROL_RINGING     3         /* Remote end is ringing */
#define AST_CONTROL_ANSWER      4         /* Remote end has answered */
#define AST_CONTROL_BUSY        5         /* Remote end is busy */
#define AST_CONTROL_TAKEOFFHOOK 6         /* Make it go off hook */
#define AST_CONTROL_OFFHOOK     7         /* Line is off hook */
#define AST_CONTROL_CONGESTION  8         /* Congestion (circuits busy) */
#define AST_CONTROL_FLASH       9         /* Flash hook */
#define AST_CONTROL_WINK        10        /* Wink */
#define AST_CONTROL_OPTION      11        /* Set an option */
#define AST_CONTROL_KEY         12        /* Key Radio */
#define AST_CONTROL_UNKEY       13        /* Unkey Radio */

#define AST_FRIENDLY_OFFSET 64 /* Reserved header space */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif

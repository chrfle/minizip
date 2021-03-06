/* mz_zip.h -- Zip manipulation
   Version 2.5.3, September 18, 2018
   part of the MiniZip project

   Copyright (C) 2010-2018 Nathan Moinvaziri
     https://github.com/nmoinvaz/minizip
   Copyright (C) 2009-2010 Mathias Svensson
     Modifications for Zip64 support
     http://result42.com
   Copyright (C) 1998-2010 Gilles Vollant
     http://www.winimage.com/zLibDll/minizip.html

   This program is distributed under the terms of the same license as zlib.
   See the accompanying LICENSE file for the full text of the license.
*/

#ifndef MZ_ZIP_H
#define MZ_ZIP_H

#include <stdint.h>
#include <time.h>

#include "mz_strm.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************/

typedef struct mz_zip_file_s
{
    uint16_t version_madeby;            // version made by
    uint16_t version_needed;            // version needed to extract
    uint16_t flag;                      // general purpose bit flag
    uint16_t compression_method;        // compression method
    time_t   modified_date;             // last modified date in unix time
    time_t   accessed_date;             // last accessed date in unix time
    time_t   creation_date;             // creation date in unix time
    uint32_t crc;                       // crc-32
    uint64_t compressed_size;           // compressed size
    uint64_t uncompressed_size;         // uncompressed size
    uint16_t filename_size;             // filename length
    uint16_t extrafield_size;           // extra field length
    uint16_t comment_size;              // file comment length
    uint32_t disk_number;               // disk number start
    uint64_t disk_offset;               // relative offset of local header
    uint16_t internal_fa;               // internal file attributes
    uint32_t external_fa;               // external file attributes
    uint16_t zip64;                     // zip64 extension mode

    const char     *filename;           // filename utf8 null-terminated string
    const uint8_t  *extrafield;         // extrafield data
    const char     *comment;            // comment utf8 null-terminated string

#ifdef HAVE_AES
    uint16_t aes_version;               // winzip aes extension if not 0
    uint8_t  aes_encryption_mode;       // winzip aes encryption mode
#endif
} mz_zip_file, mz_zip_entry;

/***************************************************************************/

typedef int32_t (*mz_zip_locate_entry_cb)(void *handle, void *userdata, mz_zip_file *file_info);

/***************************************************************************/

void *  mz_zip_create(void **handle);
// Create zip instance for opening

void    mz_zip_delete(void **handle);
// Delete zip object

int32_t mz_zip_open(void *handle, void *stream, int32_t mode);
// Create a zip file, no delete file in zip functionality

int32_t mz_zip_close(void *handle);
// Close the zip file

int32_t mz_zip_get_comment(void *handle, const char **comment);
// Get a pointer to the global comment

int32_t mz_zip_set_comment(void *handle, const char *comment);
// Set the global comment used for writing zip file

int32_t mz_zip_get_version_madeby(void *handle, uint16_t *version_madeby);
// Get the version made by

int32_t mz_zip_set_version_madeby(void *handle, uint16_t version_madeby);
// Set the version made by used for writing zip file

int32_t mz_zip_get_stream(void *handle, void **stream);
// Get a pointer to the stream used to open

/***************************************************************************/

int32_t mz_zip_entry_write_open(void *handle, const mz_zip_file *file_info,
    int16_t compress_level, uint8_t raw, const char *password);
// Open for writing the current file in the zip file

int32_t mz_zip_entry_write(void *handle, const void *buf, int32_t len);
// Write bytes from the current file in the zip file

int32_t mz_zip_entry_is_open(void *handle);
// Check to see if entry is open for read/write

int32_t mz_zip_entry_is_dir(void *handle);
// Checks to see if the entry is a directory

int32_t mz_zip_entry_read_open(void *handle, uint8_t raw, const char *password);
// Open for reading the current file in the zip file

int32_t mz_zip_entry_read(void *handle, void *buf, int32_t len);
// Read bytes from the current file in the zip file

int32_t mz_zip_entry_get_info(void *handle, mz_zip_file **file_info);
// Get info about the current file, only valid while current entry is open

int32_t mz_zip_entry_get_local_info(void *handle, mz_zip_file **local_file_info);
// Get local info about the current file, only valid while current entry is being read

int32_t mz_zip_entry_close_raw(void *handle, uint64_t uncompressed_size, uint32_t crc32);
// Close the current file in the zip file where raw is compressed data

int32_t mz_zip_entry_close(void *handle);
// Close the current file in the zip file

/***************************************************************************/

int32_t mz_zip_get_number_entry(void *handle, int64_t *number_entry);
// Get the total number of entries

int32_t mz_zip_get_disk_number_with_cd(void *handle, uint32_t *disk_number_with_cd);
// Get the the disk number containing the central directory record

int64_t mz_zip_get_entry(void *handle);
// Return offset of the current entry in the zip file

int32_t mz_zip_goto_entry(void *handle, uint64_t cd_pos);
// Go to specified entry in the zip file

int32_t mz_zip_goto_first_entry(void *handle);
// Go to the first entry in the zip file

int32_t mz_zip_goto_next_entry(void *handle);
// Go to the next entry in the zip file or MZ_END_OF_LIST if reaching the end

int32_t mz_zip_locate_entry(void *handle, const char *filename, uint8_t ignore_case);
// Locate the file with the specified name in the zip file or MZ_END_LIST if not found

int32_t mz_zip_locate_first_entry(void *handle, void *userdata, mz_zip_locate_entry_cb cb);
// Locate the first matching entry based on a match callback

int32_t mz_zip_locate_next_entry(void *handle, void *userdata, mz_zip_locate_entry_cb cb);
// LOcate the next matching entry based on a match callback

/***************************************************************************/

int32_t mz_zip_attrib_is_dir(int32_t attrib, int32_t version_madeby);
// Checks to see if the attribute is a directory based on platform

int32_t mz_zip_attrib_convert(uint8_t src_sys, int32_t src_attrib, uint8_t target_sys, int32_t *target_attrib);
// Converts file attributes from one host system to another

int32_t mz_zip_attrib_posix_to_win32(int32_t posix_attrib, int32_t *win32_attrib);
// Converts posix file attributes to win32 file attributes

int32_t mz_zip_attrib_win32_to_posix(int32_t win32_attrib, int32_t *posix_attrib);
// Converts win32 file attributes to posix file attributes

/***************************************************************************/

int32_t  mz_zip_dosdate_to_tm(uint64_t dos_date, struct tm *ptm);
// Convert dos date/time format to struct tm

time_t   mz_zip_dosdate_to_time_t(uint64_t dos_date);
// Convert dos date/time format to time_t

int32_t  mz_zip_time_t_to_tm(time_t unix_time, struct tm *ptm);
// Convert time_t to time struct

uint32_t mz_zip_time_t_to_dos_date(time_t unix_time);
// Convert time_t to dos date/time format

uint32_t mz_zip_tm_to_dosdate(const struct tm *ptm);
// Convert struct tm to dos date/time format

int32_t  mz_zip_ntfs_to_unix_time(uint64_t ntfs_time, time_t *unix_time);
// Convert ntfs time to unix time

int32_t  mz_zip_unix_to_ntfs_time(time_t unix_time, uint64_t *ntfs_time);
// Convert unix time to ntfs time

/***************************************************************************/

int32_t  mz_zip_path_compare(const char *path1, const char *path2, uint8_t ignore_case);
// Compare two paths without regard to slashes

/***************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _ZIP_H */

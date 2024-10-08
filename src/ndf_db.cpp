#include "ndf_db.hpp"
#include "sqlite_helpers.hpp"

#include <optional>
#include <spdlog/spdlog.h>

bool create_table(sqlite3 *db, const char *query) {
  SQLStatement<0, 0> stmt_create_ndf_file_tbl;
  stmt_create_ndf_file_tbl.init(db, query);
  if (!stmt_create_ndf_file_tbl.execute()) {
    return false;
  }
  return true;
}

bool NDF_DB::init_statements() {
  create_table(db,
               R"rstr( CREATE TABLE ndf_file(
                                            id INTEGER PRIMARY KEY AUTOINCREMENT,
                                            vfs_path TEXT,
                                            dat_path TEXT,
                                            fs_path TEXT,
                                            game_version TEXT,
                                            is_current BOOLEAN
                                            ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_object(
                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                          ndf_id INTEGER NOT NULL,
                                          object_name TEXT,
                                          class_name TEXT,
                                          export_path TEXT,
                                          is_top_object BOOLEAN,
                                          FOREIGN KEY (ndf_id) REFERENCES ndf_file(id) ON UPDATE CASCADE ON DELETE CASCADE
                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_property(
                                            id INTEGER PRIMARY KEY AUTOINCREMENT,
                                            object_id INTEGER NOT NULL REFERENCES ndf_object(id) ON UPDATE CASCADE ON DELETE CASCADE,
                                            property_name TEXT,
                                            property_index INTEGER,
                                            parent INTEGER REFERENCES ndf_property(id) ON UPDATE CASCADE ON DELETE CASCADE,
                                            position INTEGER,
                                            type INTEGER,
                                            is_import_reference BOOLEAN,
                                            value INTEGER
                                            ); )rstr");

  create_table(db,
               R"rstr( CREATE TABLE ndf_bool(
                                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                                          value BOOLEAN
                                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_int8(
                                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                                          value INTEGER
                                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_uint8(
                                                           id INTEGER PRIMARY KEY AUTOINCREMENT,
                                                           value INTEGER
                                                           ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_int16(
                                                           id INTEGER PRIMARY KEY AUTOINCREMENT,
                                                           value INTEGER
                                                           ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_uint16(
                                   id INTEGER PRIMARY KEY AUTOINCREMENT,
                                   value INTEGER
                                   ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_int32(
                                                           id INTEGER PRIMARY KEY AUTOINCREMENT,
                                                           value INTEGER
                                                           ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_uint32(
                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                          value INTEGER
                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_float32(
                                           id INTEGER PRIMARY KEY AUTOINCREMENT,
                                           value REAL
                                           ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_float64(
                                           id INTEGER PRIMARY KEY AUTOINCREMENT,
                                           value REAL
                                           ); )rstr");
  create_table(db, R"rstr( CREATE TABLE ndf_string(
                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                          value TEXT
                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_widestring(
                                              id INTEGER PRIMARY KEY AUTOINCREMENT,
                                              value TEXT
                                              ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_F32_vec2(
                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                          value_x REAL,
                                          value_y REAL
                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_F32_vec3(
                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                          value_x REAL,
                                          value_y REAL,
                                          value_z REAL
                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_F32_vec4(
                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                          value_x REAL,
                                          value_y REAL,
                                          value_z REAL,
                                          value_w REAL
                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_S32_vec2(
                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                          value_x INTEGER,
                                          value_y INTEGER
                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_S32_vec3(
                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                          value_x INTEGER,
                                          value_y INTEGER,
                                          value_z INTEGER
                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_S32_vec4(
                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                          value_x INTEGER,
                                          value_y INTEGER,
                                          value_z INTEGER,
                                          value_w INTEGER
                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_color(
                                                           id INTEGER PRIMARY KEY AUTOINCREMENT,
                                                           value_r INTEGER,
                                                           value_g INTEGER,
                                                           value_b INTEGER,
                                                           value_a INTEGER
                                                           ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_object_reference(
                                         id INTEGER PRIMARY KEY AUTOINCREMENT,
                                         referenced_object INTEGER REFERENCES ndf_object(id) ON UPDATE CASCADE ON DELETE SET NULL,
                                         optional_value TEXT
                                         ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_import_reference(
                                                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                                                    referenced_object INTEGER REFERENCES ndf_object(id) ON UPDATE CASCADE ON DELETE SET NULL,
                                                    optional_value TEXT
                                                    ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_GUID(
                                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                                          value TEXT
                                                          ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_path_reference(
                                                  id INTEGER PRIMARY KEY AUTOINCREMENT,
                                                  value TEXT
                                                  ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_localisation_hash(
                                                     id INTEGER PRIMARY KEY AUTOINCREMENT,
                                                     value TEXT
                                                     ); )rstr");
  create_table(db,
               R"rstr( CREATE TABLE ndf_hash(
                                                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                                                          value TEXT
                                                          ); )rstr");
  // inserters
  stmt_insert_ndf_file.init(
      db,
      R"rstr( INSERT INTO ndf_file (vfs_path, dat_path, fs_path, game_version, is_current) VALUES (?,?,?,?,?); )rstr");
  stmt_insert_ndf_object.init(
      db,
      R"rstr( INSERT INTO ndf_object (ndf_id, object_name, class_name, export_path, is_top_object) VALUES (?,?,?,?,?); )rstr");
  stmt_insert_ndf_property.init(
      db,
      R"rstr( INSERT INTO ndf_property (object_id, property_name, property_index, parent, position, type, is_import_reference, value) VALUES (?,?,?,?,?,?,?,?); )rstr");
  stmt_insert_ndf_bool.init(
      db, R"rstr( INSERT INTO ndf_bool (value) VALUES (?); )rstr");
  stmt_insert_ndf_int8.init(
      db, R"rstr( INSERT INTO ndf_int8 (value) VALUES (?); )rstr");
  stmt_insert_ndf_uint8.init(
      db, R"rstr( INSERT INTO ndf_uint8 (value) VALUES (?); )rstr");
  stmt_insert_ndf_int16.init(
      db, R"rstr( INSERT INTO ndf_int16 (value) VALUES (?); )rstr");
  stmt_insert_ndf_uint16.init(
      db, R"rstr( INSERT INTO ndf_uint16 (value) VALUES (?); )rstr");
  stmt_insert_ndf_int32.init(
      db, R"rstr( INSERT INTO ndf_int32 (value) VALUES (?); )rstr");
  stmt_insert_ndf_uint32.init(
      db, R"rstr( INSERT INTO ndf_uint32 (value) VALUES (?); )rstr");
  stmt_insert_ndf_float32.init(
      db, R"rstr( INSERT INTO ndf_float32 (value) VALUES (?); )rstr");
  stmt_insert_ndf_float64.init(
      db, R"rstr( INSERT INTO ndf_float64 (value) VALUES (?); )rstr");
  stmt_insert_ndf_string.init(
      db, R"rstr( INSERT INTO ndf_string (value) VALUES (?); )rstr");
  stmt_insert_ndf_widestring.init(
      db, R"rstr( INSERT INTO ndf_widestring (value) VALUES (?); )rstr");
  stmt_insert_ndf_F32_vec2.init(
      db,
      R"rstr( INSERT INTO ndf_F32_vec2 (value_x, value_y) VALUES (?,?); )rstr");
  stmt_insert_ndf_F32_vec3.init(
      db,
      R"rstr( INSERT INTO ndf_f32_vec3 (value_x, value_y, value_z) VALUES (?,?,?); )rstr");
  stmt_insert_ndf_F32_vec4.init(
      db,
      R"rstr( INSERT INTO ndf_F32_vec4 (value_x, value_y, value_z, value_w) VALUES (?,?,?,?); )rstr");
  stmt_insert_ndf_S32_vec2.init(
      db,
      R"rstr( INSERT INTO ndf_S32_vec2 (value_x, value_y) VALUES (?,?); )rstr");
  stmt_insert_ndf_S32_vec3.init(
      db,
      R"rstr( INSERT INTO ndf_S32_vec3 (value_x, value_y, value_z) VALUES (?,?,?); )rstr");
  stmt_insert_ndf_S32_vec4.init(
      db,
      R"rstr( INSERT INTO ndf_S32_vec4 (value_x, value_y, value_z, value_w) VALUES (?,?,?,?); )rstr");
  stmt_insert_ndf_color.init(
      db,
      R"rstr( INSERT INTO ndf_color (value_r, value_g, value_b, value_a) VALUES (?,?,?,?); )rstr");
  stmt_insert_ndf_object_reference.init(
      db,
      R"rstr( INSERT INTO ndf_object_reference (referenced_object, optional_value) VALUES (?,?); )rstr");
  stmt_insert_ndf_import_reference.init(
      db,
      R"rstr( INSERT INTO ndf_import_reference (referenced_object, optional_value) VALUES (?,?); )rstr");
  stmt_insert_ndf_GUID.init(
      db, R"rstr( INSERT INTO ndf_GUID (value) VALUES (?); )rstr");
  stmt_insert_ndf_localisation_hash.init(
      db, R"rstr( INSERT INTO ndf_localisation_hash (value) VALUES (?); )rstr");
  stmt_insert_ndf_hash.init(
      db, R"rstr( INSERT INTO ndf_hash (value) VALUES (?); )rstr");

  // accessors
  stmt_get_object_from_name.init(
      db, R"rstr( SELECT id FROM ndf_object WHERE object_name=?; )rstr");
  stmt_get_object_from_export_path.init(
      db, R"rstr( SELECT id FROM ndf_object WHERE export_path=?; )rstr");
  stmt_get_object_ndf_id.init(
      db, R"rstr( SELECT ndf_id FROM ndf_object WHERE id=?; )rstr");
  stmt_get_object_name.init(
      db, R"rstr( SELECT object_name FROM ndf_object WHERE id=?; )rstr");
  stmt_get_object_names.init(
      db, R"rstr( SELECT object_name FROM ndf_object WHERE ndf_id=?; )rstr");
  stmt_get_object_class_names.init(
      db,
      R"rstr( SELECT DISTINCT class_name FROM ndf_object WHERE ndf_id=?; )rstr");
  stmt_get_object_export_path.init(
      db, R"rstr( SELECT export_path FROM ndf_object WHERE id=?; )rstr");
  stmt_get_object.init(
      db,
      R"rstr( SELECT ndf_id, object_name, class_name, export_path, is_top_object FROM ndf_object WHERE id=?; )rstr");
  stmt_get_object_properties.init(
      db,
      R"rstr( SELECT id FROM ndf_property WHERE object_id=? AND property_index<>-1; )rstr");
  stmt_get_property_names.init(
      db,
      R"rstr( SELECT property_name FROM ndf_property WHERE object_id=? AND property_index<>-1; )rstr");

  stmt_get_property.init(
      db,
      R"rstr( SELECT object_id, property_name, property_index, parent, position, type, is_import_reference, value FROM ndf_property WHERE id=?; )rstr");

  // value accessors
  stmt_get_bool_value.init(
      db, R"rstr( SELECT value FROM ndf_bool WHERE id=?; )rstr");
  stmt_get_int8_value.init(
      db, R"rstr( SELECT value FROM ndf_int8 WHERE id=?; )rstr");
  stmt_get_uint8_value.init(
      db, R"rstr( SELECT value FROM ndf_uint8 WHERE id=?; )rstr");
  stmt_get_int16_value.init(
      db, R"rstr( SELECT value FROM ndf_int16 WHERE id=?; )rstr");
  stmt_get_uint16_value.init(
      db, R"rstr( SELECT value FROM ndf_uint16 WHERE id=?; )rstr");
  stmt_get_int32_value.init(
      db, R"rstr( SELECT value FROM ndf_int32 WHERE id=?; )rstr");
  stmt_get_uint32_value.init(
      db, R"rstr( SELECT value FROM ndf_uint32 WHERE id=?; )rstr");
  stmt_get_float32_value.init(
      db, R"rstr( SELECT value FROM ndf_float32 WHERE id=?; )rstr");
  stmt_get_float64_value.init(
      db, R"rstr( SELECT value FROM ndf_float64 WHERE id=?; )rstr");
  stmt_get_string_value.init(
      db, R"rstr( SELECT value FROM ndf_string WHERE id=?; )rstr");
  stmt_get_widestring_value.init(
      db, R"rstr( SELECT value FROM ndf_widestring WHERE id=?; )rstr");
  stmt_get_F32_vec2_value.init(
      db, R"rstr( SELECT value_x, value_y FROM ndf_F32_vec2 WHERE id=?; )rstr");
  stmt_get_F32_vec3_value.init(
      db,
      R"rstr( SELECT value_x, value_y, value_z FROM ndf_F32_vec3 WHERE id=?; )rstr");
  stmt_get_F32_vec4_value.init(
      db,
      R"rstr( SELECT value_x, value_y, value_z, value_w FROM ndf_F32_vec4 WHERE id=?; )rstr");
  stmt_get_S32_vec2_value.init(
      db, R"rstr( SELECT value_x, value_y FROM ndf_S32_vec2 WHERE id=?; )rstr");
  stmt_get_S32_vec3_value.init(
      db,
      R"rstr( SELECT value_x, value_y, value_z FROM ndf_S32_vec3 WHERE id=?; )rstr");
  stmt_get_S32_vec4_value.init(
      db,
      R"rstr( SELECT value_x, value_y, value_z, value_w FROM ndf_S32_vec4 WHERE id=?; )rstr");
  stmt_get_color_value.init(
      db,
      R"rstr( SELECT value_r, value_g, value_b, value_a FROM ndf_color WHERE id=?; )rstr");
  stmt_get_object_reference_value.init(
      db,
      R"rstr( SELECT referenced_object, optional_value FROM ndf_object_reference WHERE id=?; )rstr");
  stmt_get_import_reference_value.init(
      db,
      R"rstr( SELECT referenced_object, optional_value FROM ndf_import_reference WHERE id=?; )rstr");
  stmt_get_GUID_value.init(
      db, R"rstr( SELECT value FROM ndf_GUID WHERE id=?; )rstr");
  stmt_get_path_reference_value.init(
      db, R"rstr( SELECT value FROM ndf_path_reference WHERE id=?; )rstr");
  stmt_get_localisation_hash_value.init(
      db, R"rstr( SELECT value FROM ndf_localisation_hash WHERE id=?; )rstr");
  stmt_get_hash_value.init(
      db, R"rstr( SELECT value FROM ndf_hash WHERE id=?; )rstr");
  // used by list, map and pair
  stmt_get_list_items.init(
      db,
      R"rstr( SELECT id FROM ndf_property WHERE parent=? ORDER BY position; )rstr");
  stmt_get_objects_referencing.init(
      db,
      R"rstr( SELECT DISTINCT prop.object_id FROM ndf_object_reference AS ref INNER JOIN ndf_property AS prop ON prop.value_id=ref.id WHERE ref.referenced_object=?; )rstr");
  stmt_get_objects_importing.init(
      db,
      R"rstr( SELECT DISTINCT prop.object_id FROM ndf_import_reference AS ref INNER JOIN ndf_property AS prop ON prop.value_id=ref.id WHERE ref.referenced_object=?; )rstr");

  // change values
  stmt_set_bool_value.init(
      db, R"rstr( UPDATE ndf_bool SET value=? WHERE id=?; )rstr");
  stmt_set_int8_value.init(
      db, R"rstr( UPDATE ndf_int8 SET value=? WHERE id=?; )rstr");
  stmt_set_uint8_value.init(
      db, R"rstr( UPDATE ndf_uint8 SET value=? WHERE id=?; )rstr");
  stmt_set_int16_value.init(
      db, R"rstr( UPDATE ndf_int16 SET value=? WHERE id=?; )rstr");
  stmt_set_uint16_value.init(
      db, R"rstr( UPDATE ndf_uint16 SET value=? WHERE id=?; )rstr");
  stmt_set_int32_value.init(
      db, R"rstr( UPDATE ndf_int32 SET value=? WHERE id=?; )rstr");
  stmt_set_uint32_value.init(
      db, R"rstr( UPDATE ndf_uint32 SET value=? WHERE id=?; )rstr");
  stmt_set_float32_value.init(
      db, R"rstr( UPDATE ndf_float32 SET value=? WHERE id=?; )rstr");
  stmt_set_float64_value.init(
      db, R"rstr( UPDATE ndf_float64 SET value=? WHERE id=?; )rstr");
  stmt_set_string_value.init(
      db, R"rstr( UPDATE ndf_string SET value=? WHERE id=?; )rstr");
  stmt_set_widestring_value.init(
      db, R"rstr( UPDATE ndf_widestring SET value=? WHERE id=?; )rstr");
  stmt_set_F32_vec2_value.init(
      db,
      R"rstr( UPDATE ndf_F32_vec2 SET value_x=?, value_y=? WHERE id=?; )rstr");
  stmt_set_F32_vec3_value.init(
      db,
      R"rstr( UPDATE ndf_F32_vec3 SET value_x=?, value_y=?, value_z=? WHERE id=?; )rstr");
  stmt_set_F32_vec4_value.init(
      db,
      R"rstr( UPDATE ndf_F32_vec4 SET value_x=?, value_y=?, value_z=?, value_w=? WHERE id=?; )rstr");
  stmt_set_S32_vec2_value.init(
      db,
      R"rstr( UPDATE ndf_S32_vec2 SET value_x=?, value_y=? WHERE id=?; )rstr");
  stmt_set_S32_vec3_value.init(
      db,
      R"rstr( UPDATE ndf_S32_vec3 SET value_x=?, value_y=?, value_z=? WHERE id=?; )rstr");
  stmt_set_S32_vec4_value.init(
      db,
      R"rstr( UPDATE ndf_S32_vec4 SET value_x=?, value_y=?, value_z=?, value_w=? WHERE id=?; )rstr");
  stmt_set_color_value.init(
      db,
      R"rstr( UPDATE ndf_color SET value_r=?, value_g=?, value_b=?, value_a=? WHERE id=?; )rstr");
  stmt_set_object_reference_value.init(
      db,
      R"rstr( UPDATE ndf_object_reference SET referenced_object=?,optional_value=? WHERE id=?; )rstr");
  stmt_set_import_reference_value.init(
      db,
      R"rstr( UPDATE ndf_import_reference SET referenced_object=?,optional_value=? WHERE id=?; )rstr");
  stmt_set_GUID_value.init(
      db, R"rstr( UPDATE ndf_GUID SET value=? WHERE id=?; )rstr");
  stmt_set_path_reference_value.init(
      db, R"rstr( UPDATE ndf_path_reference SET value=? WHERE id=?; )rstr");
  stmt_set_localisation_hash_value.init(
      db, R"rstr( UPDATE ndf_localisation_hash SET value=? WHERE id=?; )rstr");
  stmt_set_hash_value.init(
      db, R"rstr( UPDATE ndf_hash SET value=? WHERE id=?; )rstr");
  // object updates
  stmt_set_object_name.init(
      db, R"rstr( UPDATE ndf_object SET object_name=? WHERE id=?; )rstr");
  stmt_set_object_export_path.init(
      db, R"rstr( UPDATE ndf_object SET export_path=? WHERE id=?; )rstr");

  stmt_update_object_references.init(db,
                                     R"rstr( UPDATE ndf_object_reference
                                             SET referenced_object=ndf_object.id
                                             FROM ndf_object
                                             WHERE referenced_object IS NULL
                                             AND optional_value=ndf_object.object_name
                                             AND ndf_object.ndf_id=?; )rstr");
  stmt_update_import_references.init(db,
                                     R"rstr( UPDATE ndf_import_reference
                                             SET referenced_object=ndf_object.id
                                             FROM ndf_object
                                             WHERE referenced_object IS NULL
                                             AND optional_value=ndf_object.export_path
                                             AND (ndf_object.ndf_id) IN
                                             (SELECT id FROM ndf_file WHERE is_current=True); )rstr");

  // delete file
  stmt_delete_ndf_file.init(db,
                            R"rstr( DELETE FROM ndf_file WHERE id=?; )rstr");

  return true;
}

bool NDF_DB::init() {
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    spdlog::error("Could not open SQLite DB: {}", sqlite3_errmsg(db));
    return false;
  }
  return init_statements();
}

bool NDF_DB::init(fs::path path) {
  std::string path_str = path.string();
  int rc = sqlite3_open(path_str.c_str(), &db);

  if (rc != SQLITE_OK) {
    spdlog::error("Could not open SQLite DB: {}", sqlite3_errmsg(db));
    return false;
  }
  return init_statements();
}

NDF_DB::~NDF_DB() {
  if (db) {
    sqlite3_close(db);
  }
}

std::optional<int> NDF_DB::insert_file(std::string vfs_path,
                                       std::string dat_path,
                                       std::string fs_path, std::string version,
                                       bool is_current) {
  return stmt_insert_ndf_file.insert(vfs_path, dat_path, fs_path, version,
                                     is_current);
}

std::optional<int> NDF_DB::insert_object(int ndf_idx, const NDFObject &object) {
  auto object_id =
      stmt_insert_ndf_object.insert(ndf_idx, object.name, object.class_name,
                                    object.export_path, object.is_top_object);
  if (!object_id.has_value()) {
    return std::nullopt;
  }

  for (auto &prop : object.properties) {
    insert_property(*prop, *object_id);
  }
  return object_id;
}

bool NDF_DB::insert_property(const NDFProperty &property, int object_id,
                             int parent, int position) {
  return property.to_ndf_db(this, object_id, parent, position);
}

std::optional<NDFObject> NDF_DB::get_object(int object_idx) {
  auto obj_data_opt = stmt_get_object.query_single<
      std::tuple<int, std::string, std::string, std::string, bool>>(object_idx);
  if (!obj_data_opt) {
    return std::nullopt;
  }
  NDFObject ret;
  auto [_, object_name, class_name, export_path, is_top_object] =
      obj_data_opt.value();
  ret.name = object_name;
  ret.class_name = class_name;
  ret.export_path = export_path;
  ret.is_top_object = is_top_object;

  // get list of all properties in this object (only direct properties, not list
  // items etc.)
  auto prop_ids_opt = stmt_get_object_properties.query<int>(object_idx);
  if (!prop_ids_opt) {
    return std::nullopt;
  }
  for (auto prop_id : prop_ids_opt.value()) {
    auto prop_opt = get_property(prop_id);
    if (prop_opt) {
      ret.properties.push_back(std::move(prop_opt.value()));
    } else {
      return std::nullopt;
    }
  }
  return ret;
}

std::optional<std::unique_ptr<NDFProperty>>
NDF_DB::get_property(int property_id) {
  // get property type
  auto property = NDFProperty::get_db_property_type(this, property_id);
  if (!property) {
    return std::nullopt;
  }
  if (!property->from_ndf_db(this, property_id)) {
    return std::nullopt;
  }
  return property;
}

bool NDF_DB::change_object_name(int object_id, std::string new_name) {
  // update to the new name
  if (!stmt_set_object_name.execute(new_name, object_id)) {
    return false;
  }
  return true;
}

bool NDF_DB::change_export_path(int object_id, std::string new_path) {
  // update to the new export_path
  if (!stmt_set_object_export_path.execute(new_path, object_id)) {
    return false;
  }
  return true;
}

bool NDF_DB::fix_references(int ndf_id) {
  // update all object references
  if (!stmt_update_object_references.execute(ndf_id)) {
    return false;
  }
  // update all import references
  if (!stmt_update_import_references.execute()) {
    return false;
  }
  return true;
}

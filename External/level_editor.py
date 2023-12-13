import bpy
import math
import bpy_extras
import gpu
import gpu_extras.batch
import copy
import mathutils
import json

bl_info = {
    "name":"レベルエディタ",
    "author": "Ryuta Abe",
    "version":(1,0),
    "blender":(3,3,1),
    "location":"",
    "description":"レベルエディタ",
    "warning":"",
    "support":"TESTING",
    "wiki_url":"",
    "tracker_url":"",
    "category":"Object",
}

class DrawCollider:
    handle = None
    def draw_collider():
        vertices = {"pos":[]}
        indices = []
        offsets = [
            [-0.5,-0.5,-0.5],
            [+0.5,-0.5,-0.5],
            [-0.5,+0.5,-0.5],
            [+0.5,+0.5,-0.5],
            [-0.5,-0.5,+0.5],
            [+0.5,-0.5,+0.5],
            [-0.5,+0.5,+0.5],
            [+0.5,+0.5,+0.5],
        ]
        size = [2,2,2]
        for object in bpy.context.scene.objects:
            if not "collider" in object:
                continue

            center = mathutils.Vector((0,0,0))
            size = mathutils.Vector((2,2,2))
            center[0] = object["collider_center"][0]
            center[1] = object["collider_center"][1]
            center[2] = object["collider_center"][2]
            size[0] = object["collider_size"][0]
            size[1] = object["collider_size"][1]
            size[2] = object["collider_size"][2]

            start = len(vertices["pos"])

            for offset in offsets:
                pos = copy.copy(center)
                pos[0] += offset[0] * size[0]
                pos[1] += offset[1] * size[1]
                pos[2] += offset[2] * size[2]
                pos = object.matrix_world @ pos
                vertices['pos'].append(pos)
                indices.append([start+0,start+1])
                indices.append([start+2,start+3])
                indices.append([start+0,start+2])
                indices.append([start+1,start+3])
                indices.append([start+4,start+5])
                indices.append([start+6,start+7])
                indices.append([start+4,start+6])
                indices.append([start+5,start+7])
                indices.append([start+0,start+4])
                indices.append([start+1,start+5])
                indices.append([start+2,start+6])
                indices.append([start+3,start+7])
        shader = gpu.shader.from_builtin("3D_UNIFORM_COLOR")
        batch = gpu_extras.batch.batch_for_shader(shader,"LINES",vertices,indices = indices)
        color = [0.5,1.0,1.0,1.0]
        shader.bind()
        shader.uniform_float("color",color)
        batch.draw(shader)



class OBJECT_PT_file_name(bpy.types.Panel):
    """オブジェクトのファイルネームパネル"""
    bl_idname = "OBJECT_PT_file_name"
    bl_label = "FileName"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self,context):
        if "file_name" in context.object:
            self.layout.prop(context.object,'["file_name"]',text=self.bl_label)
        else:
            self.layout.operator(MYADDON_OT_add_filename.bl_idname)

class OBJECT_PT_tag(bpy.types.Panel):
    """オブジェクトのタグパネル"""
    bl_idname = "OBJECT_PT_tag"
    bl_label = "Tag"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self,context):
        if "tag" in context.object:
            self.layout.prop(context.object,'["tag"]',text=self.bl_label)
        else:
            self.layout.operator(MYADDON_OT_add_tag.bl_idname)

class OBJECT_PT_modelType(bpy.types.Panel):
    """オブジェクトのModelTypeパネル"""
    bl_idname = "OBJECT_PT_modelType"
    bl_label = "ModelType"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self,context):
        if "modelType" in context.object:
            self.layout.prop(context.object,'["modelType"]',text=self.bl_label)
        else:
            self.layout.operator(MYADDON_OT_add_modelType.bl_idname)

class MYADDON_OT_add_filename(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_filename"
    bl_label = "FileName 追加"
    bl_descriptor = "['file_name']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}
    def execute(self,context):
        context.object["file_name"] = ""
        return {"FINISHED"}
    

class MYADDON_OT_add_tag(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_tag"
    bl_label = "Tag 追加"
    bl_descriptor = "['tag']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}
    def execute(self,context):
        context.object["tag"] = ""
        return {"FINISHED"}
    
class MYADDON_OT_add_modelType(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_tag2"
    bl_label = "modelType 追加"
    bl_descriptor = "['modelType']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}
    def execute(self,context):
        context.object["modelType"] = ""
        return {"FINISHED"}

class MYADDON_OT_add_box_collider(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_box_collider"
    bl_label = "コライダー 追加"
    bl_descriptor = "['collider']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}
    def execute(self,context):
        context.object["collider"] = "Box"
        context.object["collider_center"] = mathutils.Vector((0,0,0))
        context.object["collider_size"] = mathutils.Vector((2,2,2))
        return {"FINISHED"}


class MYADDON_OT_add_variable(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_variable"
    bl_label = "addVariable"
    bl_descriptor = "['variable']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}
    def execute(self,context):
        context.object["RotationSpeed"] = mathutils.Vector((0,0,0))
        context.object["MoveVec"] = mathutils.Vector((0,0,0))
        context.object["MoveMax"] = 0.0
        return {"FINISHED"}


class OBJECT_PT_variable(bpy.types.Panel):
    bl_idname = "OBJECT_PT_variable"
    bl_label = "addVariable"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self,context):
        if "variable" in context.object:
            self.layout.prop(context.object,'["RotationSpeed"]',text="RotationSpeed")
            self.layout.prop(context.object,'["MoveVec"]',text="MoveVec")
            self.layout.prop(context.object,'["MoveMax"]',text="MoveMax")
        else:
            self.layout.operator(MYADDON_OT_add_box_collider.bl_idname)


class OBJECT_PT_collider(bpy.types.Panel):
    bl_idname = "OBJECT_PT_collider"
    bl_label = "Collider"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self,context):
        if "collider" in context.object:
            self.layout.prop(context.object,'["collider"]',text="Type")
            self.layout.prop(context.object,'["collider_center"]',text="Center")
            self.layout.prop(context.object,'["collider_size"]',text="Size")
        else:
            self.layout.operator(MYADDON_OT_add_box_collider.bl_idname)


class TOPBAR_MT_my_menu(bpy.types.Menu):
    bl_idname = "TOPBAR_MT_my_menu"
    bl_label = "MyMenu"
    bl_descriptor = "拡張メニュー by" + bl_info["author"]

    def draw(self,context):
        self.layout.operator("wm.url_open_preset", text = "Manual",icon = 'HELP')
        self.layout.operator(MYADDON_OT_stretch_vertex.bl_idname,text=MYADDON_OT_stretch_vertex.bl_label)
        self.layout.operator(MYADDON_OT_create_ico_sphere.bl_idname,text=MYADDON_OT_create_ico_sphere.bl_label)
        self.layout.operator(MYADDON_OT_export_scene.bl_idname,text=MYADDON_OT_export_scene.bl_label)
    def submenu(self,context):
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)
       


class MYADDON_OT_export_scene(bpy.types.Operator,bpy_extras.io_utils.ExportHelper):
    bl_idname = "my_addon.myaddon_ot_export_scene"
    bl_label = "シーン出力"
    bl_description = "シーン情報をExportします"
    filename_ext = ".json"
    bl_options = {"REGISTER"}

    def write_and_print(self,file,str):
        print(str)
        file.write(str)
        file.write('\n')

    def parse_scene_recursive(self,file,object,level):
        """シーン解析用再起関数"""
        indent = ''
        for i in range(level):
            indent += "\t"
        self.write_and_print(file,indent + object.type)
        trans, rot, scale = object.matrix_local.decompose()
        rot = rot.to_euler()
        rot.x = math.degrees(rot.x)
        rot.y = math.degrees(rot.y)
        rot.z = math.degrees(rot.z)
        self.write_and_print(file,indent + "T %f %f %f" % (trans.x,trans.y,trans.z  ))
        self.write_and_print(file,indent + "R %f %f %f" % (rot.x,rot.y,rot.z  ))
        self.write_and_print(file,indent + "S %f %f %f" % (scale.x,scale.y,scale.z  ))
        if "file_name" in object:
            self.write_and_print(file,indent + "N %s" % object["file_name"])
        if "tag" in object:
            self.write_and_print(file,indent + "TG %s" % object["tag"])
        if "modelType" in object:
            self.write_and_print(file,indent + "MT %s" % object["modelType"])
        if "collider" in object:
            self.write_and_print(file,indent + "C %s" % object["collider"])
            temp_str = indent + "CC %f %f %f"
            temp_str %= (object["collider_center"][0],object["collider_center"][1],object["collider_center"][2])
            self.write_and_print(file,temp_str)
            temp_str = indent + "CS %f %f %f"
            temp_str %= (object["collider_size"][0],object["collider_size"][1],object["collider_size"][2])
            self.write_and_print(file,temp_str)
        self.write_and_print(file,indent + 'END')
        self.write_and_print(file,'')
        for child in object.children:
            self.parse_scene_recursive(file,child,level + 1)

    def parse_scene_recursive_json(self,data_parent,object,level):
        json_object = dict()
        json_object["Type"] = object.type
        json_object["name"] = object.name
        trans, rot, scale = object.matrix_local.decompose()
        rot = rot.to_euler()
        rot.x = math.degrees(rot.x)
        rot.y = math.degrees(rot.y)
        rot.z = math.degrees(rot.z)
        transform = dict()
        transform["translation"] = (trans.x, trans.y, trans.z)
        transform["rotation"] = (rot.x, rot.y, rot.z)
        transform["scaling"] = (scale.x,scale.y,scale.z)
        json_object["transform"] = transform
        if "file_name" in object:
            json_object["file_name"] = object["file_name"]
        if "tag" in object:
            json_object["tag"] = object["tag"]
        if "modelType" in object:
            json_object["modelType"] = object["modelType"]
        if "collider" in object:
            collider = dict()
            collider["type"] = object["collider"]
            collider["center"] = object["collider_center"].to_list()
            collider["size"] = object["collider_size"].to_list()
            json_object["collider"] = collider
        if len(object.children) > 0:
            json_object["children"] = list()
            for child in object.children:
                self.parse_scene_recursive_json(json_object["children"],child,level + 1)

        data_parent.append(json_object)

    def export_json(self):
        """json形式出力"""
        json_object_root = dict()
        json_object_root["name"] = "scene"
        json_object_root["objects"] = list()
        for object in bpy.context.scene.objects:
            if (object.parent):
                continue
            self.parse_scene_recursive_json(json_object_root["objects"],object,0)

        json_text = json.dumps(json_object_root,ensure_ascii=False,cls=json.JSONEncoder,indent=4)
        print(json_text)
        with open(self.filepath,"wt",encoding="utf-8") as file:
            file.write(json_text)

    def export(self):
        """ファイルに出力"""
        print("シーン情報出力開始... %r" % self.filepath)
        with open(self.filepath,"wt") as file:
            file.write("SCENE\n")
            for object in bpy.context.scene.objects:
                if (object.parent):
                    continue
                    """self.write_and_print(file,"Parent:" + object.parent.name)"""
                self.parse_scene_recursive(file,object,0)

    def execute(self, context):
        print("シーン情報をExportします")
        self.export_json()
       
        print("シーン情報をExportしました")
        self.report({"INFO"},"シーン情報をExportしました")
        return {"FINISHED"}


class MYADDON_OT_create_ico_sphere(bpy.types.Operator):
    bl_idname = "my_addon.myaddon_ot_create_sphere"
    bl_label = "ICO球生成"
    bl_description = "ICO球を生成します"
    bl_options = {"REGISTER",'UNDO'}


    def execute(self, context):
        bpy.ops.mesh.primitive_ico_sphere_add()
        print("ICO球を生成しました。")
        return {"FINISHED"}


class MYADDON_OT_stretch_vertex(bpy.types.Operator):
    bl_idname = "my_addon.myaddon_ot_streych_vertex"
    bl_label = "頂点を伸ばす"
    bl_description = "頂点データを引っ張って伸ばします"
    bl_options = {'REGISTER','UNDO'}


    def execute(self, context):
        bpy.data.objects["Cube"].data.vertices[0].co.x += 1.0
        print("頂点を伸ばしました。")
        return {"FINISHED"}


classes = (MYADDON_OT_export_scene,
           MYADDON_OT_create_ico_sphere,
           MYADDON_OT_stretch_vertex,
           TOPBAR_MT_my_menu,
           MYADDON_OT_add_filename,
           OBJECT_PT_file_name,
           MYADDON_OT_add_box_collider,
           OBJECT_PT_collider,
           MYADDON_OT_add_tag,
           OBJECT_PT_tag,
           MYADDON_OT_add_modelType,
           OBJECT_PT_modelType)

def draw_menu_manual(self,context):
    self.layout.operator("wm.url_open_preset",text="Manual",icon='HELP')
def register():
    for cls in classes:
        bpy.utils.register_class(cls)
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    DrawCollider.handle = bpy.types.SpaceView3D.draw_handler_add(DrawCollider.draw_collider,(),"WINDOW","POST_VIEW")
    print("レベルエディタが有効化されました")
def unregister():
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    bpy.types.SpaceView3D.draw_handler_remove(DrawCollider.handle,"WINDOW")
    for cls in classes:
        bpy.utils.unregister_class(cls)
    print("レベルエディタが無効化されました")


if __name__ == "__main__":
    register()
#include "MooMooEngine_PCH.h"
#include "FileLoader.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace MooMooEngine 
{
    // https://www.youtube.com/watch?v=zn7N7zHgCcs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=93
    // Function to open a file with a given filter
    std::string FileLoader::OpenFile(const char* filter)
    {
        // Initialize the OPENFILENAMEA struct
        OPENFILENAMEA fileName;
        CHAR szFile[260] = { 0 };       // Buffer for the file name
        CHAR currentDir[256] = { 0 };   // Buffer for the current directory
        ZeroMemory(&fileName, sizeof(OPENFILENAME));
        fileName.lStructSize = sizeof(OPENFILENAME);

        // Get the window handle from GLFW and set it as the owner
        fileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
        fileName.lpstrFile = szFile;
        fileName.nMaxFile = sizeof(szFile);

        // Set the initial directory to the current directory
        if (GetCurrentDirectoryA(256, currentDir))
            fileName.lpstrInitialDir = currentDir;

        fileName.lpstrFilter = filter;      // Set the filter for the file types
        fileName.nFilterIndex = 1;          // Use the first filter in the list
        fileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        // Open the file dialog and return the selected file path if successful
        if (GetOpenFileNameA(&fileName) == TRUE)
            return fileName.lpstrFile;

        // Returns an empty string if no file was selected
        return std::string();
    }

	std::string FileLoader::SaveFile(const char* filter)
	{
		OPENFILENAMEA fileName;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&fileName, sizeof(OPENFILENAME));
		fileName.lStructSize = sizeof(OPENFILENAME);
		fileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		fileName.lpstrFile = szFile;
		fileName.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			fileName.lpstrInitialDir = currentDir;
		fileName.lpstrFilter = filter;
		fileName.nFilterIndex = 1;
		fileName.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		fileName.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&fileName) == TRUE)
			return fileName.lpstrFile;

		// Return empty sting if not valid
		return std::string();
	}

    void FileLoader::JSONSave(entt::registry& scene, std::string location)
    {        
        std::ofstream outputStream(location);

        if (!outputStream.is_open()) {
            std::cerr << "Failed to open the file: " << location << std::endl;
            return;
        }

        Json::Value root;

        int i = 0;

        // Goes through all the entities
        auto view = scene.view<TagComponent>();
        for (auto entity : view) {
        const auto& tag = scene.get<TagComponent>(entity);
        Json::Value& object = root["GameObjects"][tag.Tag.c_str()];

            // Check if the entity has a TagComponent, which it will.
            if (scene.any_of<TagComponent>(entity)) {
                const auto& tag = scene.get<TagComponent>(entity);
                root["GameObjects"][tag.Tag.c_str()]["Tag"] = tag.Tag;


                if (scene.any_of<ShapeComponent>(entity)) {
                    const auto& shape = scene.get<ShapeComponent>(entity);
                    object["Shape component"]["Shape Type"] = shape.shape;
                }

                if (scene.any_of<ModelComponent>(entity)) {
                    const auto& modelComponent = scene.get<ModelComponent>(entity);
                    
                    object["Model component"]["Model Path"] = modelComponent.model.GetPath();
                }

                // Retrieves the TransformComponent values
                if (scene.any_of<TransformComponent>(entity)) {
                    const auto& transform = scene.get<TransformComponent>(entity);
                    object["Transform component"]["Position"] = Json::Value(Json::arrayValue);
                    object["Transform component"]["Position"].append(transform.position.x);
                    object["Transform component"]["Position"].append(transform.position.y);
                    object["Transform component"]["Position"].append(transform.position.z);
                    
                    object["Transform component"]["Rotation"] = Json::Value(Json::arrayValue);
                    object["Transform component"]["Rotation"].append(transform.rotation.x);
                    object["Transform component"]["Rotation"].append(transform.rotation.y);
                    object["Transform component"]["Rotation"].append(transform.rotation.z);
                    
                    object["Transform component"]["Scale"] = Json::Value(Json::arrayValue);
                    object["Transform component"]["Scale"].append(transform.scale.x);
                    object["Transform component"]["Scale"].append(transform.scale.y);
                    object["Transform component"]["Scale"].append(transform.scale.z);
                }       

                if (scene.any_of<MaterialComponent>(entity)) {
                    auto& material = scene.get<MaterialComponent>(entity);
                    object["Material component"]["Colour"] = Json::Value(Json::arrayValue);
                    object["Material component"]["Colour"].append(material.colour.r);
                    object["Material component"]["Colour"].append(material.colour.g);
                    object["Material component"]["Colour"].append(material.colour.b);
                    object["Material component"]["Colour"].append(material.colour.a);

                    //object["Material component"]["Has Material"]   = material.hasMatComp;
                    //object["Material component"]["Has Texture"]    = material.hasTexture;
                    object["Material component"]["Material Path"] = material.textPath;
                }
                

                if (scene.any_of<PhysicsRBDComponent>(entity)) {
                    const auto& rb = scene.get<PhysicsRBDComponent>(entity);
                    object["Physics component"]["Gravity Enabled"] = rb.gravityEnabled;
                    object["Physics component"]["Is Kinematic"] = rb.isKinematic;
                }

                if (scene.any_of<BoxColliderComponent>(entity)) {
                    const auto& boxCollider = scene.get<BoxColliderComponent>(entity);
                    object["Box Collider component"]["Position"] = Json::Value(Json::arrayValue);
                    object["Box Collider component"]["Position"].append(boxCollider.position.x);
                    object["Box Collider component"]["Position"].append(boxCollider.position.y);
                    object["Box Collider component"]["Position"].append(boxCollider.position.z);
                    
                    object["Box Collider component"]["Half Extents"] = Json::Value(Json::arrayValue);
                    object["Box Collider component"]["Half Extents"].append(boxCollider.halfExtents.x);
                    object["Box Collider component"]["Half Extents"].append(boxCollider.halfExtents.y);
                    object["Box Collider component"]["Half Extents"].append(boxCollider.halfExtents.z);
                    
                    object["Box Collider component"]["Is Trigger"] = boxCollider.isTrigger;
                }

                if (scene.any_of<SphereColliderComponent>(entity)) {
                    const auto& sphereCollider = scene.get<SphereColliderComponent>(entity);

                    object["Sphere Collider component"]["Radius"] = sphereCollider.radius;
                    object["Sphere Collider component"]["Is Trigger"] = sphereCollider.isTrigger;

                    if (scene.any_of<TransformComponent>(entity)) {
                        const auto& transform = scene.get<TransformComponent>(entity);

                        object["Sphere Collider component"]["Position"] = Json::Value(Json::arrayValue);
                        object["Sphere Collider component"]["Position"].append(transform.position.x);
                        object["Sphere Collider component"]["Position"].append(transform.position.y);
                        object["Sphere Collider component"]["Position"].append(transform.position.z);
                    }
                }
            }
        }

        Json::StreamWriterBuilder builder;
        const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(root, &outputStream);
        outputStream.close();
    }

    void FileLoader::JSONLoad(std::string location, Ref<Scene> scene, Ref<MeshRegistry> m_MeshRegistry, Ref<Shader> shader, PhysicsLayer* m_physics)
    {
        //std::string location = "assets/GameScene.json";
        std::ifstream inputStream(location);

        if (!inputStream.is_open()) {
            std::cerr << "Failed to open the file: " << location << std::endl;
            return;
        }

        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse(inputStream, root);
        inputStream.close();

        if (!parsingSuccessful) {
            std::cerr << "Failed to parse JSON file: " << reader.getFormattedErrorMessages() << std::endl;
            return;
        }

        if (!root.isMember("GameObjects") || !root["GameObjects"].isObject()) {
            std::cerr <<" No 'GameObjects." << std::endl;
            return;        }

        // Iterate through "GameObjects"
        Json::Value gameObjects = root["GameObjects"];

        if (gameObjects.isObject()) {
            for (const auto& object : gameObjects) {
                std::string defaultTag = "object";  // Default
                Entity entity = scene->CreateEntity(defaultTag);

                if (object.isMember("Tag")) {
                    const Json::Value& tagValue = object["Tag"];
                    if (tagValue.isString()) {
                        const char* tagChars = tagValue.asCString();  // Get the C-string from JSON
                        std::string tag(tagChars);  // Construct a std::string from the C-string

                        if (entity.HasComponent<TagComponent>()) {
                            entity.GetComponent<TagComponent>().Tag = tag;  // Set the tag if TagComponent exists
                        }
                    }
                }

                // Retrieve and add TransformComponent
                if (object.isMember("Transform component")) {
                    const Json::Value& transform = object["Transform component"];
                    glm::vec3 position(transform["Position"][0].asFloat(), transform["Position"][1].asFloat(), transform["Position"][2].asFloat());
                    glm::vec3 rotation(transform["Rotation"][0].asFloat(), transform["Rotation"][1].asFloat(), transform["Rotation"][2].asFloat());
                    glm::vec3 scale(transform["Scale"][0].asFloat(), transform["Scale"][1].asFloat(), transform["Scale"][2].asFloat());

                    entity.GetComponent<TransformComponent>().Translate(position);
                    entity.GetComponent<TransformComponent>().Rotation(rotation);
                    entity.GetComponent<TransformComponent>().Scale(scale);
                }

                if (object.isMember("Material component")) {
                    const Json::Value& material = object["Material component"];
                    glm::vec4 colour(material["Colour"][0].asFloat(), material["Colour"][1].asFloat(), material["Colour"][2].asFloat(), material["Colour"][3].asFloat());

                    entity.AddComponent<MaterialComponent>(shader);
                    auto& matComp = entity.GetComponent<MaterialComponent>();

                    matComp.Colour(colour);

                    const char* texturePath = material["Material Path"].asCString();
                    int length = strlen(texturePath);
                    if (length > 1) {
                        matComp.hasTexture = true;
                        matComp.textPath = texturePath;

                        Texture2D texture2D;
                        matComp.texID = texture2D.LoadTexture(texturePath);
                    }
                }

                if (object.isMember("Physics component")) {
                    const Json::Value& physics = object["Physics component"];
                    bool gravityEnabled = physics["Gravity Enabled"].asBool();
                    bool isKinematic = physics["Is Kinematic"].asBool();

                    const Json::Value& transform = object["Transform component"];
                    glm::vec3 position(transform["Position"][0].asFloat(), transform["Position"][1].asFloat(), transform["Position"][2].asFloat());

                    DynamicRigidBody* RBD = new DynamicRigidBody(m_physics->physics, m_physics->scene, position);

                    entity.AddComponent<PhysicsRBDComponent>(RBD);
                    entity.GetComponent<PhysicsRBDComponent>().gravityEnabled = gravityEnabled;
                    entity.GetComponent<PhysicsRBDComponent>().isKinematic = isKinematic;
                }

                if (object.isMember("Box Collider component")) {
                    const Json::Value& collider = object["Box Collider component"];
                    glm::vec3 position(collider["Position"][0].asFloat(), collider["Position"][1].asFloat(), collider["Position"][2].asFloat());
                    glm::vec3 halfExtents(collider["Half Extents"][0].asFloat(), collider["Half Extents"][1].asFloat(), collider["Half Extents"][2].asFloat());
                    bool isTrigger = collider["Is Trigger"].asBool();

                    const Json::Value& transform = object["Transform component"];
                    glm::vec3 scale(transform["Scale"][0].asFloat(), transform["Scale"][1].asFloat(), transform["Scale"][2].asFloat());

                    // Retrieve DynamicRigidBody from the PhysicsRBDComponent
                    auto& physicsComponent = entity.GetComponent<PhysicsRBDComponent>();
                    DynamicRigidBody* rigidBody = physicsComponent.rbD;


                    if (rigidBody) {
                        PhysXBoxCollider* boxColl = new PhysXBoxCollider(m_physics->physics, m_physics->scene, rigidBody->rbD, rigidBody->baseMaterial, scale / 2.0f);

                        entity.AddComponent<BoxColliderComponent>(boxColl);
                        auto& boxColliderComponent = entity.GetComponent<BoxColliderComponent>();
                        boxColliderComponent.position = position;
                        boxColliderComponent.halfExtents = halfExtents;
                        boxColliderComponent.isTrigger = isTrigger;
                    }
                    else {
                        std::cerr << "Error: RigidBody is null. BoxColliderComponent cannot be created." << std::endl;
                    }
                }

                if (object.isMember("Sphere Collider component")) {
                    const Json::Value& collider = object["Sphere Collider component"];
                    float radius = collider["Radius"].asFloat();
                    bool isTrigger = collider["Is Trigger"].asBool();

                    const Json::Value& transform = object["Transform component"];
                    glm::vec3 scale(transform["Scale"][0].asFloat(), transform["Scale"][1].asFloat(), transform["Scale"][2].asFloat());

                    // Retrieve DynamicRigidBody from the PhysicsRBDComponent
                    auto& physicsComponent = entity.GetComponent<PhysicsRBDComponent>();
                    DynamicRigidBody* rigidBody = physicsComponent.rbD;

                    if (rigidBody) {
                        float sphereRadius = glm::max(scale.x, glm::max(scale.y, scale.z)) / 2.0f;

                        PhysXSphereCollider* sphereColl = new PhysXSphereCollider(m_physics->physics, m_physics->scene, rigidBody->rbD, rigidBody->baseMaterial, sphereRadius);

                        entity.AddComponent<SphereColliderComponent>(sphereColl);
                        auto& sphereColliderComponent = entity.GetComponent<SphereColliderComponent>();
                        sphereColliderComponent.radius = radius;
                        sphereColliderComponent.isTrigger = isTrigger;
                    }
                    else {
                        std::cerr << "Error: RigidBody is null. SphereColliderComponent cannot be created." << std::endl;
                    }
                }

                if (object.isMember("Model component")) {
                    const Json::Value& modelData = object["Model component"];
                    const char* modelPath = modelData["Model Path"].asCString();

                    // Find the position of "\\MooMooEngine\\" in the modelPath
                    //std::string searchString = "\\MooMooEngine\\";
                    //std::string fullPath = modelPath;
                    //size_t pos = fullPath.find(searchString);

                    //if (pos != std::string::npos) {
                        // Starting from pos + length of searchString so after the //
                        //std::string relativePath = fullPath.substr(pos + searchString.length());

                        // relativePath
                        //std::cout << "Relative path: " << relativePath << std::endl;


                        Model model(modelPath, m_MeshRegistry, shader);
                        entity.AddComponent<ModelComponent>(model);
                    //}

                }

                // Add ShapeComponent
                if (object.isMember("Shape component")) {
                    const Json::Value& shape = object["Shape component"];
                    int shapeType = shape["Shape Type"].asInt();

                    entity.AddComponent<ShapeComponent>(shapeType);
                }
            }
        }

        //Entity triangle = scene->CreateEntity("Triangle");
        //triangle.AddComponent<ShapeComponent>(0);
        //triangle.AddComponent<MaterialComponent>(shader);
        //triangle.GetComponent<MaterialComponent>().Colour(glm::vec4(1));
        //triangle.GetComponent<MaterialComponent>().hasMatComp = true;
    }

}
#include "MooMooEngine.h"
#include "Scene/StateManager.h"

using namespace std;

namespace MooMooEngine
{

	// Forward declaration of the ScriptComponent
	struct ScriptComponent;

	class ScriptManager
	{
	public:
        // Singleton instance access
        static ScriptManager& Instance()
        {
            static ScriptManager instance;
            return instance;
        }

        // Add a new script component
        void AddScriptComponent(int entityId, const std::string& scriptName)
        {
            auto scriptComponent = std::make_shared<ScriptComponent>(scriptName);
            scripts[entityId] = scriptComponent;
            std::cout << "Script component \"" << scriptName << "\" added to entity " << entityId << "." << std::endl;
        }

        // Get a script component by entity ID
        std::shared_ptr<ScriptComponent> GetScriptComponent(int entityId)
        {
            auto it = scripts.find(entityId);
            if (it != scripts.end())
            {
                return it->second;
            }
            else
            {
                std::cout << "Script component for entity " << entityId << " not found." << std::endl;
                return nullptr;
            }
        }

        // Remove a script component by entity ID
        void RemoveScriptComponent(int entityId)
        {
            if (scripts.erase(entityId))
            {
                std::cout << "Script component for entity " << entityId << " removed successfully." << std::endl;
            }
            else
            {
                std::cout << "Script component for entity " << entityId << " not found." << std::endl;
            }
        }

        // Update all script components (example update method)
        void UpdateScripts()
        {
            for (auto& [entityId, scriptComponent] : scripts)
            {
                // Perform script update logic here
                std::cout << "Updating script: " << scriptComponent->name << " for entity " << entityId << std::endl;
            }
        }

    private:
        // Private constructor for Singleton pattern
        ScriptManager() {}

        // Disable copy and assignment
        ScriptManager(const ScriptManager&) = delete;
        ScriptManager& operator=(const ScriptManager&) = delete;

        // Container to store script components
        std::unordered_map<int, std::shared_ptr<ScriptComponent>> scripts;
	};
}
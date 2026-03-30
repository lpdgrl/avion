#ifndef AVION_CORE_LIGHT_H
#define AVION_CORE_LIGHT_H 

    #include "../../glm/glm.hpp"
    #include "object.hpp"

    #include <memory>

    namespace avion::core  {
        
        /* struct Light {
            glm::vec3 position;
        
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
        }; */ 

        class ILight;

        struct LightParams {
            ILight* light;
            Color color;
            Size size;
        };

        // Non-Virtual-Interface (NVI) idiom
        class ILight {
        public:
            ILight() = default;

            ILight(const ILight& other) = delete;
            ILight& operator=(const ILight& other) = delete;
            
            ILight(ILight&& other) = delete;
            ILight& operator=(ILight&& other) = delete;


            glm::vec3 GetAmbient() const;
            glm::vec3 GetDiffuse() const;
            glm::vec3 GetSpecular() const;
            
            glm::vec3& GetGeometry();

            void SetAmbient(glm::vec3 ambient);
            void SetDiffuse(glm::vec3 diffuse);
            void SetSpecular(glm::vec3 specular);
            
            void SetParams(); 

            virtual ~ILight() = default;
        private:
            virtual glm::vec3 GetAmbientImpl() const = 0;
            virtual glm::vec3 GetDiffuseImpl() const = 0;
            virtual glm::vec3 GetSpecularImpl() const = 0;

            virtual void SetAmbientImpl(glm::vec3 ambient) = 0;
            virtual void SetDiffuseImpl(glm::vec3 diffuse) = 0;
            virtual void SetSpecularImpl(glm::vec3 specular) = 0;
        
            virtual void SetParamsImpl() = 0;

            virtual glm::vec3& GetGeometryImpl() = 0;
        };

        class DirLight : public ILight {
        public:
            DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

            glm::vec3 GetAmbientImpl() const override; 
            glm::vec3 GetDiffuseImpl() const override;
            glm::vec3 GetSpecularImpl() const override;
            
            void SetAmbientImpl(glm::vec3 ambient) override;
            void SetDiffuseImpl(glm::vec3 diffuse) override;
            void SetSpecularImpl(glm::vec3 specular) override;

            void SetParamsImpl() override; 
            
            glm::vec3& GetGeometryImpl() override;

            void SetDirection(glm::vec3 direction) noexcept;
            glm::vec3 GetDirection() const noexcept;

           ~DirLight();
        private:
            glm::vec3 direction_;
            
            glm::vec3 ambient_;
            glm::vec3 diffuse_;
            glm::vec3 specular_;
        };

        class PointLight : public ILight {
        public:
            PointLight(
                    glm::vec3 position, 
                    glm::vec3 ambient, 
                    glm::vec3 diffuse, 
                    glm::vec3 specular, 
                    float constant,
                    float linear,
                    float quadratic
            );
            
            glm::vec3 GetAmbientImpl() const override;
            glm::vec3 GetDiffuseImpl() const override;
            glm::vec3 GetSpecularImpl() const override;

            void SetAmbientImpl(glm::vec3 ambient) override;
            void SetDiffuseImpl(glm::vec3 diffuse) override;
            void SetSpecularImpl(glm::vec3 specular) override;

            void SetParamsImpl() override;

            glm::vec3& GetGeometryImpl() override;

            void SetPosition(glm::vec3 position) noexcept;
            glm::vec3 GetPosition() const noexcept; 
           
            void SetConstant(float constant) noexcept;
            float GetConstant() const noexcept;

            void SetLinear(float linear) noexcept;
            float GetLinear() const noexcept;

            void SetQuadratic(float quadratic) noexcept;
            float GetQuadratic() const noexcept;
            
            ~PointLight();

        private:
            glm::vec3 position_;

            glm::vec3 ambient_;
            glm::vec3 diffuse_;
            glm::vec3 specular_;

            float constant_;
            float linear_;
            float quadratic_;
        };



    } // namespace avion::core 

#endif // AVION_CORE_LIGHT_H

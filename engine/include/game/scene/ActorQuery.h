#pragma once
#include <type_traits>

#include "game/UI/UIScreen.h"
#include "game/object/Actor.h"

template <typename... T>
struct TypeLists {};

// 判定用traits
template <typename T>
struct IsTypeLists : std::false_type {};

template <typename... Ts>
struct IsTypeLists<TypeLists<Ts...>> : std::true_type {};

struct ActorQueryDeps {
    /*
        Sceneの生成ではactorの存在が前提となっているため，
        ObjectsSystemBaseではなく，
        ActorsSystemの方を依存関係に含める
    */
    class ActorsSystem& actorsSystem;
    class RenderDB& renderDB;
    class AudioSystem& audioSystem;
    class PhysWorld& physWorld;
    class UISystem& uiSystem;
    class StateManager& stateManager;
    class RuntimeRequestManager& runtimeReqManager;
};

class ActorQuery {
    ActorQueryDeps mSystems;

    template <typename T>
    T& GetSystem();

    // TDeps -> TypeLists を解決するための補助関数
    template <typename T, typename... Ts>
    T ResolveDeps(TypeLists<Ts...> _) {
        return T(GetSystem<Ts>()...);
    }

   public:
    ActorQuery(ActorQueryDeps aqd) : mSystems(aqd) {}

    // todo: 上手くテンプレートメタプログラミングを行えば，TActorのみの指定で良くなるらしい
    // Actorクラスの内部にTDepsやTListsの型指定を行う．このときstatic_assertで型が定義されているかをチェックする必要がある．
    /*
        TActor: Actorの具体クラス
        TDeps: Actorの具体クラスが引数に取る依存システムの参照をまとめた構造体．必ずコンストラクタを定義する必要がある．
        TLists: TDepsに注入したいシステムの型のリスト．TDepsが取る引数の順番に指定する必要がある．
    */
    template <typename TActor, typename TDeps, typename TLists>
    ActorID CreateActor() {
        static_assert(std::is_base_of<Actor, TActor>::value,
                      "TActor must derive from Actor");
        static_assert(IsTypeLists<TLists>::value,
                      "TListss must be a TypeLists<...>");

        // 依存関係depsの解決
        TDeps deps = ResolveDeps<TDeps>(TLists{});

        // Actor生成
        TActor* actor = new TActor(&mSystems.actorsSystem, deps);

        return actor->GetID();
    }

    template <typename TUI, typename TDeps, typename TLists>
    UIID CreateUI() {
        static_assert(std::is_base_of<UIScreen, TUI>::value,
                      "TActor must derive from UIScreen");
        static_assert(IsTypeLists<TLists>::value,
                      "TListss must be a TypeLists<...>");

        // 依存関係depsの解決
        TDeps deps = ResolveDeps<TDeps>(TLists{});

        // UI生成
        TUI* ui = new TUI(&mSystems.uiSystem, deps);

        return ui->GetID();
    }

    template <typename TActor>
    TActor* GetActor(ActorID id) {
        return mSystems.actorsSystem.GetActor<TActor>(id);
    }

    template <typename TUI>
    TUI* GetUI(UIID id) {
        return mSystems.uiSystem.GetUI<TUI>(id);
    }
};

template <>
inline class ActorsSystem& ActorQuery::GetSystem<ActorsSystem>() {
    return mSystems.actorsSystem;
}

template <>
inline class RenderDB& ActorQuery::GetSystem<RenderDB>() {
    return mSystems.renderDB;
}

template <>
inline class AudioSystem& ActorQuery::GetSystem<AudioSystem>() {
    return mSystems.audioSystem;
}

template <>
inline class PhysWorld& ActorQuery::GetSystem<PhysWorld>() {
    return mSystems.physWorld;
}

template <>
inline class UISystem& ActorQuery::GetSystem<UISystem>() {
    return mSystems.uiSystem;
}

template <>
inline class StateManager& ActorQuery::GetSystem<StateManager>() {
    return mSystems.stateManager;
}

template <>
inline class RuntimeRequestManager&
ActorQuery::GetSystem<RuntimeRequestManager>() {
    return mSystems.runtimeReqManager;
}

# Adaptateur Unity

Unity peut réutiliser `AstraCore` comme bibliothèque native C++ ou reproduire son contrat dans un service C# équivalent. Le dépôt ne choisit pas de moteur Unity et ne dépend pas du package Unity.

Le wrapper doit exposer :

- `StartMission(scenarioId)` ;
- `ObservePoint(pointId, hasSpectrometer, hasCamera)` ;
- `SpendResource(resource, amount)` ;
- `FinishMission()` ;
- `GetResourceState()` et `GetMissionReport()`.

Les `ScriptableObject` ou fichiers JSON Unity doivent conserver les identifiants des scénarios et points de `data/scenarios/scenarios.json`. Les scènes et l’interface peuvent être réimplémentées sans modifier les données NASA documentées dans `docs/NASA_DATA_SOURCES.md`.
